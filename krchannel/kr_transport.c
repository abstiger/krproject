#include "krutils/kr_utils.h"
#include "kr_transport.h"


T_KRTransport* kr_transport_new(T_KRTransportConfig *config)
{
    T_KRTransport *transport = kr_calloc(sizeof(*transport));
    if (transport == NULL) {
        KR_LOG(KR_LOGERROR, "calloc transport failed!");
        return NULL;
    }

    transport->config = config;
    transport->module = kr_module_open(config->module_path, RTLD_LAZY);
    if (transport->module == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_open %s failed!", config->module_path);
        kr_free(transport);
        return NULL;
    }

    /*
    T_KRTransportRegisterFunc reg_func = \
        kr_module_symbol(transport->module, config->register_func);
    if (reg_func == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol %s failed!", \
                config->register_func);
        kr_free(transport);
        return NULL;
    }
    transport->func = (T_KRTransportFunc *)reg_func();
    */

    return transport;
}

void kr_transport_free(T_KRTransport *transport)
{
    if (transport != NULL) {
        kr_module_close(transport->module);
        kr_free(transport);
    }
}

