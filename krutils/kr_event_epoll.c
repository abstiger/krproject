#include <sys/epoll.h>

typedef struct _kr_event_api_state_t {
    int epfd;
    struct epoll_event *events;
} T_KREventApiState;

static int kr_event_api_create(T_KREventLoop *eventLoop) {
    T_KREventApiState *state = kr_malloc(sizeof(T_KREventApiState));

    if (!state) return -1;
    state->events = kr_malloc(sizeof(struct epoll_event)*eventLoop->setsize);
    if (!state->events) {
        kr_free(state);
        return -1;
    }
    state->epfd = epoll_create(1024); /* 1024 is just an hint for the kernel */
    if (state->epfd == -1) {
        kr_free(state->events);
        kr_free(state);
        return -1;
    }
    eventLoop->apidata = state;
    return 0;
}

static void kr_event_api_free(T_KREventLoop *eventLoop) {
    T_KREventApiState *state = eventLoop->apidata;

    close(state->epfd);
    kr_free(state->events);
    kr_free(state);
}

static int kr_event_api_add(T_KREventLoop *eventLoop, int fd, int mask) {
    T_KREventApiState *state = eventLoop->apidata;
    struct epoll_event ee;
    /* If the fd was already monitored for some event, we need a MOD
     * operation. Otherwise we need an ADD operation. */
    int op = eventLoop->events[fd].mask == KR_EVENT_NONE ?
            EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    ee.events = 0;
    mask |= eventLoop->events[fd].mask; /* Merge old events */
    if (mask & KR_EVENT_READABLE) ee.events |= EPOLLIN;
    if (mask & KR_EVENT_WRITABLE) ee.events |= EPOLLOUT;
    ee.data.u64 = 0; /* avoid valgrind warning */
    ee.data.fd = fd;
    if (epoll_ctl(state->epfd,op,fd,&ee) == -1) return -1;
    return 0;
}

static void kr_event_api_del(T_KREventLoop *eventLoop, int fd, int delmask) {
    T_KREventApiState *state = eventLoop->apidata;
    struct epoll_event ee;
    int mask = eventLoop->events[fd].mask & (~delmask);

    ee.events = 0;
    if (mask & KR_EVENT_READABLE) ee.events |= EPOLLIN;
    if (mask & KR_EVENT_WRITABLE) ee.events |= EPOLLOUT;
    ee.data.u64 = 0; /* avoid valgrind warning */
    ee.data.fd = fd;
    if (mask != KR_EVENT_NONE) {
        epoll_ctl(state->epfd,EPOLL_CTL_MOD,fd,&ee);
    } else {
        /* Note, Kernel < 2.6.9 requires a non null event pointer even for
         * EPOLL_CTL_DEL. */
        epoll_ctl(state->epfd,EPOLL_CTL_DEL,fd,&ee);
    }
}

static int kr_event_api_poll(T_KREventLoop *eventLoop, struct timeval *tvp) {
    T_KREventApiState *state = eventLoop->apidata;
    int retval, numevents = 0;

    retval = epoll_wait(state->epfd,state->events,eventLoop->setsize,
            tvp ? (tvp->tv_sec*1000 + tvp->tv_usec/1000) : -1);
    if (retval > 0) {
        int j;

        numevents = retval;
        for (j = 0; j < numevents; j++) {
            int mask = 0;
            struct epoll_event *e = state->events+j;

            if (e->events & EPOLLIN) mask |= KR_EVENT_READABLE;
            if (e->events & EPOLLOUT) mask |= KR_EVENT_WRITABLE;
            eventLoop->fired[j].fd = e->data.fd;
            eventLoop->fired[j].mask = mask;
        }
    }
    return numevents;
}

static char *kr_event_api_name(void) {
    return "epoll";
}
