#include "kr_server.h"
#include "krdata/kr_data.h"

//TODO:add handler for server layer

static void kr_server_clion(T_KRContext *krctx, T_KREngineArg *krarg)
{

}

static void kr_server_clioff(T_KRContext *krctx, T_KREngineArg *krarg)
{

}

static void kr_server_heartbeat(T_KRContext *krctx, T_KREngineArg *krarg)
{

}


int kr_server_register_default(T_KRServer *krserver)
{
    T_KREngine *krengine = krserver->krengine;
    
    kr_engine_register(krengine, "cli_on", \
                    (KRHandleFunc )kr_server_clion);
    kr_engine_register(krengine, "cli_off", \
                    (KRHandleFunc )kr_server_clioff);
    kr_engine_register(krengine, "heart_beat", \
                    (KRHandleFunc )kr_server_heartbeat);

    return 0;
};

