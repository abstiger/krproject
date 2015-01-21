#include "kr_server.h"
#include "krdata/kr_data.h"


static void kr_server_svron(T_KRContext *krctx, T_KREngineArg *krarg)
{

}

static void kr_server_svroff(T_KRContext *krctx, T_KREngineArg *krarg)
{

}

static void kr_server_clion(T_KRContext *krctx, T_KREngineArg *krarg)
{

}

static void kr_server_clioff(T_KRContext *krctx, T_KREngineArg *krarg)
{

}

static void kr_server_heart(T_KRContext *krctx, T_KREngineArg *krarg)
{

}


int kr_server_register_default(T_KRServer *krserver)
{
    T_KREngine *krengine = krserver->krengine;

    kr_engine_register(krengine, "svr_on", (KRHandleFunc )kr_server_svron);
    kr_engine_register(krengine, "svr_off", (KRHandleFunc )kr_server_svroff);

    kr_engine_register(krengine, "cli_on", (KRHandleFunc )kr_server_clion);
    kr_engine_register(krengine, "cli_off", (KRHandleFunc )kr_server_clioff);

    kr_engine_register(krengine, "heart", (KRHandleFunc )kr_server_heart);

    return 0;
};

