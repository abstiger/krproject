#ifndef __KR_TRANSPORT_H__
#define __KR_TRANSPORT_H__


#include "kr_protocol.h"

typedef struct _kr_transport_func_t T_KRTransportFunc;

typedef struct _kr_transport_config_t
{
    char   *transport;           /* transport name */
    char   *addr;                /* Bind address or NULL */
    int     port;                /* TCP listening port */
    int     method;              /* Simplex or Duplex */
    int     timeout;             /* Timeout setting */

    char   *module_path;         /* module path */
    char   *register_func;       /* open function name */
    T_KRTransportFunc *func;     /* transport function */
}T_KRTransportConfig;


/*transport function definition*/
typedef void* (*KRTransportRegisterFunc)(void);
typedef void* (*KRTransportOpenFunc)(T_KRTransportConfig *transport_config);
typedef void (*KRTransportCloseFunc)(void *data);
typedef void (*KRTransportErrorFunc)(void *data);
typedef void (*KRTransportTimeoutFunc)(void *data);
typedef int (*KRTransportSendFunc)(void *data, T_KRBuffer *buffer);
typedef int (*KRTransportRecvFunc)(void *data, T_KRBuffer *buffer);


struct _kr_transport_func_t
{
    const char                 *name;
    KRTransportOpenFunc         open;
    KRTransportCloseFunc        close;
    KRTransportSendFunc         send;
    KRTransportRecvFunc         recv;
    KRTransportTimeoutFunc      timeout;
    KRTransportErrorFunc        error;
};


typedef struct _kr_transport_t
{
    T_KRTransportConfig *config;
    T_KRTransportFunc *func;

    T_KRModule *module;

    void *data;
    /*private variables*/
    int status;     
}T_KRTransport;


/* functions of transport */
extern T_KRTransport* kr_transport_new(T_KRTransportConfig *config);
extern void kr_transport_free(T_KRTransport *transport);


#endif  /*__KR_TRANSPORT_H__*/

