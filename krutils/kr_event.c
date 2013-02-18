#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

#include "kr_config.h"
#include "kr_event.h"
#include "kr_alloc.h"

/* Include the best multiplexing layer supported by this system.
 * The following should be ordered by performances, descending. */
#ifdef HAVE_EPOLL_CTL
#include "kr_event_epoll.c"
#else
    #ifdef HAVE_KQUEUE
    #include "kr_event_kqueue.c"
    #else
    #include "kr_event_select.c"
    #endif
#endif

T_KREventLoop *kr_event_loop_create(int setsize) {
    T_KREventLoop *eventLoop;
    int i;

    if ((eventLoop = kr_malloc(sizeof(*eventLoop))) == NULL) goto err;
    eventLoop->events = kr_malloc(sizeof(T_KRFileEvent)*setsize);
    eventLoop->fired = kr_malloc(sizeof(T_KRFiredEvent)*setsize);
    if (eventLoop->events == NULL || eventLoop->fired == NULL) goto err;
    eventLoop->setsize = setsize;
    eventLoop->timeEventHead = NULL;
    eventLoop->timeEventNextId = 0;
    eventLoop->stop = 0;
    eventLoop->maxfd = -1;
    eventLoop->beforesleep = NULL;
    if (kr_event_api_create(eventLoop) == -1) goto err;
    /* Events with mask == KR_EVENT_NONE are not set. So let's initialize the
     * vector with it. */
    for (i = 0; i < setsize; i++)
        eventLoop->events[i].mask = KR_EVENT_NONE;
    return eventLoop;

err:
    if (eventLoop) {
        kr_free(eventLoop->events);
        kr_free(eventLoop->fired);
        kr_free(eventLoop);
    }
    return NULL;
}

void kr_event_loop_delete(T_KREventLoop *eventLoop) {
    kr_event_api_free(eventLoop);
    kr_free(eventLoop->events);
    kr_free(eventLoop->fired);
    kr_free(eventLoop);
}

void kr_event_loop_stop(T_KREventLoop *eventLoop) {
    eventLoop->stop = 1;
}

int kr_event_file_create(T_KREventLoop *eventLoop, int fd, int mask,
        KREventFileFunc *proc, void *clientData)
{
    if (fd >= eventLoop->setsize) return KR_EVENT_ERR;
    T_KRFileEvent *fe = &eventLoop->events[fd];

    if (kr_event_api_add(eventLoop, fd, mask) == -1)
        return KR_EVENT_ERR;
    fe->mask |= mask;
    if (mask & KR_EVENT_READABLE) fe->rfileProc = proc;
    if (mask & KR_EVENT_WRITABLE) fe->wfileProc = proc;
    fe->clientData = clientData;
    if (fd > eventLoop->maxfd)
        eventLoop->maxfd = fd;
    return KR_EVENT_OK;
}

void kr_event_file_delete(T_KREventLoop *eventLoop, int fd, int mask)
{
    if (fd >= eventLoop->setsize) return;
    T_KRFileEvent *fe = &eventLoop->events[fd];

    if (fe->mask == KR_EVENT_NONE) return;
    fe->mask = fe->mask & (~mask);
    if (fd == eventLoop->maxfd && fe->mask == KR_EVENT_NONE) {
        /* Update the max fd */
        int j;

        for (j = eventLoop->maxfd-1; j >= 0; j--)
            if (eventLoop->events[j].mask != KR_EVENT_NONE) break;
        eventLoop->maxfd = j;
    }
    kr_event_api_del(eventLoop, fd, mask);
}

int kr_event_file_get(T_KREventLoop *eventLoop, int fd) {
    if (fd >= eventLoop->setsize) return 0;
    T_KRFileEvent *fe = &eventLoop->events[fd];

    return fe->mask;
}

static void aeGetTime(long *seconds, long *milliseconds)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    *seconds = tv.tv_sec;
    *milliseconds = tv.tv_usec/1000;
}

static void aeAddMillisecondsToNow(long long milliseconds, long *sec, long *ms) {
    long cur_sec, cur_ms, when_sec, when_ms;

    aeGetTime(&cur_sec, &cur_ms);
    when_sec = cur_sec + milliseconds/1000;
    when_ms = cur_ms + milliseconds%1000;
    if (when_ms >= 1000) {
        when_sec ++;
        when_ms -= 1000;
    }
    *sec = when_sec;
    *ms = when_ms;
}

long long kr_event_time_create(T_KREventLoop *eventLoop, long long milliseconds,
        KREventTimeFunc *proc, void *clientData,
        KREventFinalizerFunc *finalizerProc)
{
    long long id = eventLoop->timeEventNextId++;
    T_KRTimeEvent *te;

    te = kr_malloc(sizeof(*te));
    if (te == NULL) return KR_EVENT_ERR;
    te->id = id;
    aeAddMillisecondsToNow(milliseconds,&te->when_sec,&te->when_ms);
    te->timeProc = proc;
    te->finalizerProc = finalizerProc;
    te->clientData = clientData;
    te->next = eventLoop->timeEventHead;
    eventLoop->timeEventHead = te;
    return id;
}

int kr_event_time_delete(T_KREventLoop *eventLoop, long long id)
{
    T_KRTimeEvent *te, *prev = NULL;

    te = eventLoop->timeEventHead;
    while(te) {
        if (te->id == id) {
            if (prev == NULL)
                eventLoop->timeEventHead = te->next;
            else
                prev->next = te->next;
            if (te->finalizerProc)
                te->finalizerProc(eventLoop, te->clientData);
            kr_free(te);
            return KR_EVENT_OK;
        }
        prev = te;
        te = te->next;
    }
    return KR_EVENT_ERR; /* NO event with the specified ID found */
}

/* Search the first timer to fire.
 * This operation is useful to know how many time the select can be
 * put in sleep without to delay any event.
 * If there are no timers NULL is returned.
 *
 * Note that's O(N) since time events are unsorted.
 * Possible optimizations (not needed by Redis so far, but...):
 * 1) Insert the event in order, so that the nearest is just the head.
 *    Much better but still insertion or deletion of timers is O(N).
 * 2) Use a skiplist to have this operation as O(1) and insertion as O(log(N)).
 */
static T_KRTimeEvent *aeSearchNearestTimer(T_KREventLoop *eventLoop)
{
    T_KRTimeEvent *te = eventLoop->timeEventHead;
    T_KRTimeEvent *nearest = NULL;

    while(te) {
        if (!nearest || te->when_sec < nearest->when_sec ||
                (te->when_sec == nearest->when_sec &&
                 te->when_ms < nearest->when_ms))
            nearest = te;
        te = te->next;
    }
    return nearest;
}

/* Process time events */
static int processTimeEvents(T_KREventLoop *eventLoop) {
    int processed = 0;
    T_KRTimeEvent *te;
    long long maxId;

    te = eventLoop->timeEventHead;
    maxId = eventLoop->timeEventNextId-1;
    while(te) {
        long now_sec, now_ms;
        long long id;

        if (te->id > maxId) {
            te = te->next;
            continue;
        }
        aeGetTime(&now_sec, &now_ms);
        if (now_sec > te->when_sec ||
            (now_sec == te->when_sec && now_ms >= te->when_ms))
        {
            int retval;

            id = te->id;
            retval = te->timeProc(eventLoop, id, te->clientData);
            processed++;
            /* After an event is processed our time event list may
             * no longer be the same, so we restart from head.
             * Still we make sure to don't process events registered
             * by event handlers itself in order to don't loop forever.
             * To do so we saved the max ID we want to handle.
             *
             * FUTURE OPTIMIZATIONS:
             * Note that this is NOT great algorithmically. Redis uses
             * a single time event so it's not a problem but the right
             * way to do this is to add the new elements on head, and
             * to flag deleted elements in a special way for later
             * deletion (putting references to the nodes to delete into
             * another linked list). */
            if (retval != KR_EVENT_NOMORE) {
                aeAddMillisecondsToNow(retval,&te->when_sec,&te->when_ms);
            } else {
                kr_event_time_delete(eventLoop, id);
            }
            te = eventLoop->timeEventHead;
        } else {
            te = te->next;
        }
    }
    return processed;
}

/* Process every pending time event, then every pending file event
 * (that may be registered by time event callbacks just processed).
 * Without special flags the function sleeps until some file event
 * fires, or when the next time event occurrs (if any).
 *
 * If flags is 0, the function does nothing and returns.
 * if flags has KR_EVENT_ALL_EVENTS set, all the kind of events are processed.
 * if flags has KR_EVENT_FILE_EVENTS set, file events are processed.
 * if flags has KR_EVENT_TIME_EVENTS set, time events are processed.
 * if flags has KR_EVENT_DONT_WAIT set the function returns ASAP until all
 * the events that's possible to process without to wait are processed.
 *
 * The function returns the number of events processed. */
int kr_event_process(T_KREventLoop *eventLoop, int flags)
{
    int processed = 0, numevents;

    /* Nothing to do? return ASAP */
    if (!(flags & KR_EVENT_TIME_EVENTS) && !(flags & KR_EVENT_FILE_EVENTS)) return 0;

    /* Note that we want call select() even if there are no
     * file events to process as long as we want to process time
     * events, in order to sleep until the next time event is ready
     * to fire. */
    if (eventLoop->maxfd != -1 ||
        ((flags & KR_EVENT_TIME_EVENTS) && !(flags & KR_EVENT_DONT_WAIT))) {
        int j;
        T_KRTimeEvent *shortest = NULL;
        struct timeval tv, *tvp;

        if (flags & KR_EVENT_TIME_EVENTS && !(flags & KR_EVENT_DONT_WAIT))
            shortest = aeSearchNearestTimer(eventLoop);
        if (shortest) {
            long now_sec, now_ms;

            /* Calculate the time missing for the nearest
             * timer to fire. */
            aeGetTime(&now_sec, &now_ms);
            tvp = &tv;
            tvp->tv_sec = shortest->when_sec - now_sec;
            if (shortest->when_ms < now_ms) {
                tvp->tv_usec = ((shortest->when_ms+1000) - now_ms)*1000;
                tvp->tv_sec --;
            } else {
                tvp->tv_usec = (shortest->when_ms - now_ms)*1000;
            }
            if (tvp->tv_sec < 0) tvp->tv_sec = 0;
            if (tvp->tv_usec < 0) tvp->tv_usec = 0;
        } else {
            /* If we have to check for events but need to return
             * ASAP because of KR_EVENT_DONT_WAIT we need to se the timeout
             * to zero */
            if (flags & KR_EVENT_DONT_WAIT) {
                tv.tv_sec = tv.tv_usec = 0;
                tvp = &tv;
            } else {
                /* Otherwise we can block */
                tvp = NULL; /* wait forever */
            }
        }

        numevents = kr_event_api_poll(eventLoop, tvp);
        for (j = 0; j < numevents; j++) {
            T_KRFileEvent *fe = &eventLoop->events[eventLoop->fired[j].fd];
            int mask = eventLoop->fired[j].mask;
            int fd = eventLoop->fired[j].fd;
            int rfired = 0;

        /* note the fe->mask & mask & ... code: maybe an already processed
             * event removed an element that fired and we still didn't
             * processed, so we check if the event is still valid. */
            if (fe->mask & mask & KR_EVENT_READABLE) {
                rfired = 1;
                fe->rfileProc(eventLoop,fd,fe->clientData,mask);
            }
            if (fe->mask & mask & KR_EVENT_WRITABLE) {
                if (!rfired || fe->wfileProc != fe->rfileProc)
                    fe->wfileProc(eventLoop,fd,fe->clientData,mask);
            }
            processed++;
        }
    }
    /* Check time events */
    if (flags & KR_EVENT_TIME_EVENTS)
        processed += processTimeEvents(eventLoop);

    return processed; /* return the number of processed file/time events */
}

/* Wait for millseconds until the given file descriptor becomes
 * writable/readable/exception */
int kr_event_wait(int fd, int mask, long long milliseconds) {
    struct pollfd pfd;
    int retmask = 0, retval;

    memset(&pfd, 0, sizeof(pfd));
    pfd.fd = fd;
    if (mask & KR_EVENT_READABLE) pfd.events |= POLLIN;
    if (mask & KR_EVENT_WRITABLE) pfd.events |= POLLOUT;

    if ((retval = poll(&pfd, 1, milliseconds))== 1) {
        if (pfd.revents & POLLIN) retmask |= KR_EVENT_READABLE;
        if (pfd.revents & POLLOUT) retmask |= KR_EVENT_WRITABLE;
        return retmask;
    } else {
        return retval;
    }
}

void kr_event_loop_run(T_KREventLoop *eventLoop) {
    eventLoop->stop = 0;
    while (!eventLoop->stop) {
        if (eventLoop->beforesleep != NULL)
            eventLoop->beforesleep(eventLoop);
        kr_event_process(eventLoop, KR_EVENT_ALL_EVENTS);
    }
}

char *kr_event_get_api_name(void) {
    return kr_event_api_name();
}

void kr_event_set_before_sleep_proc(T_KREventLoop *eventLoop, KREventBeforeSleepFunc *beforesleep) {
    eventLoop->beforesleep = beforesleep;
}
