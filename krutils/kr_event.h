#ifndef __KR_EVENT_H__
#define __KR_EVENT_H__

#define KR_EVENT_OK 0
#define KR_EVENT_ERR -1

#define KR_EVENT_NONE 0
#define KR_EVENT_READABLE 1
#define KR_EVENT_WRITABLE 2

#define KR_EVENT_FILE_EVENTS 1
#define KR_EVENT_TIME_EVENTS 2
#define KR_EVENT_ALL_EVENTS (KR_EVENT_FILE_EVENTS|KR_EVENT_TIME_EVENTS)
#define KR_EVENT_DONT_WAIT 4

#define KR_EVENT_NOMORE -1

/* Macros */
#define KR_EVENT_NOTUSED(V) ((void) V)

typedef struct _kr_event_loop_t T_KREventLoop;

/* Types and data structures */
typedef void KREventFileFunc(T_KREventLoop *eventLoop, int fd, void *clientData, int mask);
typedef int KREventTimeFunc(T_KREventLoop *eventLoop, long long id, void *clientData);
typedef void KREventFinalizerFunc(T_KREventLoop *eventLoop, void *clientData);
typedef void KREventBeforeSleepFunc(T_KREventLoop *eventLoop);

/* File event structure */
typedef struct _kr_file_event_t {
    int mask; /* one of KR_EVENT_(READABLE|WRITABLE) */
    KREventFileFunc *rfileProc;
    KREventFileFunc *wfileProc;
    void *clientData;
} T_KRFileEvent;

/* Time event structure */
typedef struct _kr_time_event_t {
    long long id; /* time event identifier. */
    long when_sec; /* seconds */
    long when_ms; /* milliseconds */
    KREventTimeFunc *timeProc;
    KREventFinalizerFunc *finalizerProc;
    void *clientData;
    struct _kr_time_event_t *next;
} T_KRTimeEvent;

/* A fired event */
typedef struct _kr_fired_event_t {
    int fd;
    int mask;
} T_KRFiredEvent;

/* State of an event based program */
struct _kr_event_loop_t {
    int maxfd;   /* highest file descriptor currently registered */
    int setsize; /* max number of file descriptors tracked */
    long long timeEventNextId;
    T_KRFileEvent *events; /* Registered events */
    T_KRFiredEvent *fired; /* Fired events */
    T_KRTimeEvent *timeEventHead;
    int stop;
    void *apidata; /* This is used for polling API specific data */
    KREventBeforeSleepFunc *beforesleep;
};

/* Prototypes */
T_KREventLoop *kr_event_loop_create(int setsize);
void kr_event_loop_delete(T_KREventLoop *eventLoop);
void kr_event_loop_run(T_KREventLoop *eventLoop);
void kr_event_loop_stop(T_KREventLoop *eventLoop);

int kr_event_file_create(T_KREventLoop *eventLoop, int fd, int mask,
        KREventFileFunc *proc, void *clientData);
void kr_event_file_delete(T_KREventLoop *eventLoop, int fd, int mask);
int kr_event_file_get(T_KREventLoop *eventLoop, int fd);

long long kr_event_time_create(T_KREventLoop *eventLoop, long long milliseconds,
        KREventTimeFunc *proc, void *clientData,
        KREventFinalizerFunc *finalizerProc);
int kr_event_time_delete(T_KREventLoop *eventLoop, long long id);

int kr_event_process(T_KREventLoop *eventLoop, int flags);
int kr_event_wait(int fd, int mask, long long milliseconds);

char *kr_event_get_api_name(void);
void kr_event_set_before_sleep_proc(T_KREventLoop *eventLoop, KREventBeforeSleepFunc *beforesleep);

#endif /* __KR_EVENT_H__ */
