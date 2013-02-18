#include "kr_server.h"
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

T_KRServer krserver = {0};

extern void kr_server_tcp_accept_handler(T_KREventLoop *el, int fd, void *privdata, int mask);
extern void kr_server_message_read_handler(T_KREventLoop *el, int fd, void *privdata, int mask);

extern int kr_server_config_parse(char *configfile, T_KRServer *server);
extern void kr_server_config_dump(T_KRServer *server, FILE *fp);
extern void kr_server_config_free(T_KRServer *server);


static void kr_server_usage(void)
{
    fprintf(stderr, "Usage: ./krserver -c[your configure file] [-h]\n");
}


static int kr_server_argument_parse(int argc, char *argv[])
{
    int opt;
    
    while ((opt = getopt(argc, argv, "c:h")) != -1)
    {
         switch(opt)
         {
             case 'c':
                 krserver.configfile = kr_strdup(optarg);
                 break;
             case 'h':
                 kr_server_usage();
                 break;
             default:
                 fprintf(stderr, "Unrecognized option: -%c.", optopt);
                 kr_server_usage();
                 return -1;
         }
    }
    
    return 0;
}



static void kr_server_daemonize(void) 
{
    int fd;

    if (fork() != 0) exit(0); 
    setsid();
    if (fork() != 0) exit(0);

    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO) close(fd);
    }
    umask(0);
}

static void kr_server_sigterm_action(int sig) 
{
    KR_LOG(KR_LOGWARNING, "Received SIGTERM[%d], scheduling shutdown...", sig);
    krserver.shutdown = 1;
}

void kr_server_sigterm_setup(void) 
{
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = kr_server_sigterm_action;
    sigaction(SIGTERM, &act, NULL);
}


int kr_server_cron(T_KREventLoop *el, long long id, void *data)
{
    /*
    KR_LOG(KR_LOGDEBUG, "memory info: used:[%d], rss:[%d], radio:[%f]!\n", \
        kr_malloc_used_memory(), kr_malloc_get_rss(), \
        kr_malloc_get_fragmentation_ratio());
    */    
    if (krserver.shutdown == 1) {
        KR_LOG(KR_LOGWARNING, "Shutdown krserver...");
        krserver.shutdown = 0;
        kr_server_finalize();
    }
    return 500;
}


static int kr_server_try_connect_to_cluster(void)
{
    if (krserver.coordport != 0) {
        krserver.cofd = kr_net_tcp_connect(krserver.neterr, \
            krserver.coordip, krserver.coordport);
        if (krserver.cofd == KR_NET_ERR) {
            KR_LOG(KR_LOGERROR, "kr_net_tcp_connect[%s][%d] failed[%s]",
                krserver.coordip, krserver.coordport, krserver.neterr);
            return -1;
        }
    }
    
    if (krserver.cofd < 0) {
        KR_LOG(KR_LOGERROR, "Can't connect to coordinator in cluster!");
        return -1;
    }
    
    /* write svron message to the cluster */
    if (kr_server_handle_svron() != 0) {
        KR_LOG(KR_LOGERROR, "kr_server_handle_svron cluster!");
        return -1;
    }
    
    /* Register read file event */
    if (kr_event_file_create(krserver.krel, krserver.cofd, KR_EVENT_READABLE, \
        kr_server_message_read_handler, NULL) == KR_NET_ERR) {
        KR_LOG(KR_LOGERROR, "kr_event_file_create failed[%s]", krserver.neterr);
        return -1;
    }
    
    return 0;
}


int kr_server_connect_to_cluster(void)
{
    int i = 0;
    
    for (i = 0; i < krserver.retrytimes ; ++i) {
        /* try to connect to the cluster*/
        if (kr_server_try_connect_to_cluster() == 0) {
            return 0;
        }
        
        /* sleep a while then go next try*/
        sleep(krserver.retryinterval);
    }
    
    return -1;
}


int kr_server_initialize(void)
{
    int ret = 0;
    
    /* signal handling */
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    kr_server_sigterm_setup();
    
    /* Daemonize */
    if (krserver.daemonize) {
        kr_server_daemonize();
    }
    
    /* Start up krengine */
    krserver.krengine = kr_engine_startup(
            krserver.dbname, krserver.dbuser, krserver.dbpass,
            krserver.logpath, krserver.serverid, krserver.loglevel,
            krserver.shmkey, krserver.serverid, 
            krserver.krdbmodule, krserver.datamodule, krserver.rulemodule,
            krserver.hdicachesize, krserver.threadcnt, krserver.hwm);
    if (krserver.krengine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_engine_startup failed!\n");
        return -1;
    }
        
    /* Create event loop */
    krserver.krel = kr_event_loop_create(krserver.maxevents);

    /* Register server cron time event */
    kr_event_time_create(krserver.krel, 1, kr_server_cron, NULL, NULL);
    
    /* Becoming a server */
    if (krserver.tcpport != 0) {
        krserver.ipfd = kr_net_tcp_server(krserver.neterr, \
            krserver.tcpport, krserver.tcpbindaddr);
        if (krserver.ipfd == KR_NET_ERR) {
            KR_LOG(KR_LOGERROR, "kr_net_tcp_server[%d] failed[%s]",
                krserver.tcpport, krserver.neterr);
            return -1;
        }
    }
    if (krserver.ipfd < 0) {
        KR_LOG(KR_LOGERROR, "tcpport [%d] uncorrect!", krserver.tcpport);
        return -1;
    }
    
    /* Register tcp accept file event */
    ret = kr_event_file_create(krserver.krel, krserver.ipfd, 
            KR_EVENT_READABLE,  kr_server_tcp_accept_handler, NULL);
    if (ret == KR_NET_ERR) {
        KR_LOG(KR_LOGERROR, "kr_event_file_create tcp error");
        return -1;
    }
    
    /* Serve as part of cluster */
    if (krserver.clustermode != 0) {
        if (kr_server_connect_to_cluster() != 0) {
            KR_LOG(KR_LOGERROR, "kr_server_connect_to_cluster failed");
            return -1;
        }
    }
    
    return 0;
}


int kr_server_finalize()
{
    /* event loop stop */
    kr_event_loop_stop(krserver.krel);

    /* kr_db_dump */
    /*
    FILE *fpKRDBDump = NULL;
    char caDateTime[14+1] = {0};
    char caKRDBDumpFileName[1024]= {0};
    snprintf(caKRDBDumpFileName, sizeof(caKRDBDumpFileName), \
             "KRDB.%s.Dump", kr_time_system(caDateTime));
    if ((fpKRDBDump = fopen(caKRDBDumpFileName, "w")) != NULL) {
        kr_db_dump(fpKRDBDump, krserver.krdb, 0);
        fclose(fpKRDBDump);
    }
    */
    
    /* Shutdown krengine */
    kr_engine_shutdown(krserver.krengine);

    /* Server config free */
    kr_server_config_dump(&krserver, stdout);
    kr_server_config_free(&krserver);
    
    return 0;
}


int main(int argc, char *argv[])
{
    int ret = 0;
    
    /* Parse arguments to get the configure file name */
    ret = kr_server_argument_parse(argc, argv);
    if (ret != 0) {
        fprintf(stderr, "kr_server_argument_parse failed!\n");
        exit(1);
    }
    
    /* Parse configure file to the global structure */
    ret = kr_server_config_parse(krserver.configfile, &krserver);
    if (ret != 0) {
        fprintf(stderr, "kr_server_parse_configfile [%s] failed!\n", \
                krserver.configfile);
        exit(1);
    }
    
    /* initialize krserver */
    ret = kr_server_initialize();
    if (ret != 0) {
        fprintf(stderr, "kr_server_initialize failed!\n");
        exit(1);
    }
    
    /* Run the main event loop */
    kr_event_loop_run(krserver.krel);
    
    /* event loop delete */
    kr_event_loop_delete(krserver.krel);

    exit(0);
}
