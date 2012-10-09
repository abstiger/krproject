#include <string.h>

typedef struct _kr_event_api_state_t {
    fd_set rfds, wfds;
    /* We need to have a copy of the fd sets as it's not safe to reuse
     * FD sets after select(). */
    fd_set _rfds, _wfds;
} T_KREventApiState;

static int kr_event_api_create(T_KREventLoop *eventLoop) {
    T_KREventApiState *state = kr_malloc(sizeof(T_KREventApiState));

    if (!state) return -1;
    FD_ZERO(&state->rfds);
    FD_ZERO(&state->wfds);
    eventLoop->apidata = state;
    return 0;
}

static void kr_event_api_free(T_KREventLoop *eventLoop) {
    kr_free(eventLoop->apidata);
}

static int kr_event_api_add(T_KREventLoop *eventLoop, int fd, int mask) {
    T_KREventApiState *state = eventLoop->apidata;

    if (mask & KR_EVENT_READABLE) FD_SET(fd,&state->rfds);
    if (mask & KR_EVENT_WRITABLE) FD_SET(fd,&state->wfds);
    return 0;
}

static void kr_event_api_del(T_KREventLoop *eventLoop, int fd, int mask) {
    T_KREventApiState *state = eventLoop->apidata;

    if (mask & KR_EVENT_READABLE) FD_CLR(fd,&state->rfds);
    if (mask & KR_EVENT_WRITABLE) FD_CLR(fd,&state->wfds);
}

static int kr_event_api_poll(T_KREventLoop *eventLoop, struct timeval *tvp) {
    T_KREventApiState *state = eventLoop->apidata;
    int retval, j, numevents = 0;

    memcpy(&state->_rfds,&state->rfds,sizeof(fd_set));
    memcpy(&state->_wfds,&state->wfds,sizeof(fd_set));

    retval = select(eventLoop->maxfd+1,
                &state->_rfds,&state->_wfds,NULL,tvp);
    if (retval > 0) {
        for (j = 0; j <= eventLoop->maxfd; j++) {
            int mask = 0;
            T_KRFileEvent *fe = &eventLoop->events[j];

            if (fe->mask == KR_EVENT_NONE) continue;
            if (fe->mask & KR_EVENT_READABLE && FD_ISSET(j,&state->_rfds))
                mask |= KR_EVENT_READABLE;
            if (fe->mask & KR_EVENT_WRITABLE && FD_ISSET(j,&state->_wfds))
                mask |= KR_EVENT_WRITABLE;
            eventLoop->fired[numevents].fd = j;
            eventLoop->fired[numevents].mask = mask;
            numevents++;
        }
    }
    return numevents;
}

static char *kr_event_api_name(void) {
    return "select";
}
