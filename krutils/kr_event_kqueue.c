#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

typedef struct _kr_event_api_state_t {
    int kqfd;
    struct kevent *events;
} T_KREventApiState;

static int kr_event_api_create(T_KREventLoop *eventLoop) {
    T_KREventApiState *state = kr_malloc(sizeof(T_KREventApiState));

    if (!state) return -1;
    state->events = kr_malloc(sizeof(struct kevent)*eventLoop->setsize);
    if (!state->events) {
        kr_free(state);
        return -1;
    }
    state->kqfd = kqueue();
    if (state->kqfd == -1) {
        kr_free(state->events);
        kr_free(state);
        return -1;
    }
    eventLoop->apidata = state;
    
    return 0;    
}

static void kr_event_api_free(T_KREventLoop *eventLoop) {
    T_KREventApiState *state = eventLoop->apidata;

    close(state->kqfd);
    kr_free(state->events);
    kr_free(state);
}

static int kr_event_api_add(T_KREventLoop *eventLoop, int fd, int mask) {
    T_KREventApiState *state = eventLoop->apidata;
    struct kevent ke;
    
    if (mask & KR_EVENT_READABLE) {
        EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        if (kevent(state->kqfd, &ke, 1, NULL, 0, NULL) == -1) return -1;
    }
    if (mask & KR_EVENT_WRITABLE) {
        EV_SET(&ke, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
        if (kevent(state->kqfd, &ke, 1, NULL, 0, NULL) == -1) return -1;
    }
    return 0;
}

static void kr_event_api_del(T_KREventLoop *eventLoop, int fd, int mask) {
    T_KREventApiState *state = eventLoop->apidata;
    struct kevent ke;

    if (mask & KR_EVENT_READABLE) {
        EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(state->kqfd, &ke, 1, NULL, 0, NULL);
    }
    if (mask & KR_EVENT_WRITABLE) {
        EV_SET(&ke, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        kevent(state->kqfd, &ke, 1, NULL, 0, NULL);
    }
}

static int kr_event_api_poll(T_KREventLoop *eventLoop, struct timeval *tvp) {
    T_KREventApiState *state = eventLoop->apidata;
    int retval, numevents = 0;

    if (tvp != NULL) {
        struct timespec timeout;
        timeout.tv_sec = tvp->tv_sec;
        timeout.tv_nsec = tvp->tv_usec * 1000;
        retval = kevent(state->kqfd, NULL, 0, state->events, eventLoop->setsize,
                        &timeout);
    } else {
        retval = kevent(state->kqfd, NULL, 0, state->events, eventLoop->setsize,
                        NULL);
    }

    if (retval > 0) {
        int j;
        
        numevents = retval;
        for(j = 0; j < numevents; j++) {
            int mask = 0;
            struct kevent *e = state->events+j;
            
            if (e->filter == EVFILT_READ) mask |= KR_EVENT_READABLE;
            if (e->filter == EVFILT_WRITE) mask |= KR_EVENT_WRITABLE;
            eventLoop->fired[j].fd = e->ident; 
            eventLoop->fired[j].mask = mask;           
        }
    }
    return numevents;
}

static char *kr_event_api_name(void) {
    return "kqueue";
}
