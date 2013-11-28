#include "kr_coordi.h"
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

T_KRCoordi krcoordi = {0};

extern void kr_coordi_tcp_accept_handler(T_KREventLoop *el, int fd, void *priv, int mask);

extern int kr_coordi_config_parse(char *configfile, T_KRCoordi *coordi);
extern void kr_coordi_config_dump(T_KRCoordi *coordi, FILE *fp);
extern void kr_coordi_config_reset(T_KRCoordi *coordi);

static int kr_coordi_finalize();


static void kr_coordi_usage(void)
{
    fprintf(stderr, "Usage: ./krcoordi -c[your configure file] [-h]\n");
}


static int kr_coordi_argument_parse(int argc, char *argv[])
{
    int opt;
    
    while ((opt = getopt(argc, argv, "c:h")) != -1)
    {
         switch(opt)
         {
             case 'c':
                 krcoordi.configfile = kr_strdup(optarg);
                 break;
             case 'h':
                 kr_coordi_usage();
                 break;
             default:
                 fprintf(stderr, "Unrecognized option: -%c.", optopt);
                 kr_coordi_usage();
                 return -1;
         }
    }
    
    return 0;
}



static void kr_coordi_daemonize(void) 
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


static void kr_coordi_sigterm_action(int sig) 
{
    KR_LOG(KR_LOGWARNING, "Received SIGTERM[%d], scheduling shutdown...", sig);
    krcoordi.shutdown = 1;
}


void kr_coordi_sigterm_setup(void) 
{
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = kr_coordi_sigterm_action;
    sigaction(SIGTERM, &act, NULL);
}


int kr_coordi_cron(T_KREventLoop *el, long long id, void *data)
{
    /*
    KR_LOG(KR_LOGDEBUG, "memory info: used:[%d], rss:[%d], radio:[%f]!\n", \
        kr_malloc_used_memory(), kr_malloc_get_rss(), \
        kr_malloc_get_fragmentation_ratio());
    */
    if (krcoordi.shutdown == 1) {
        KR_LOG(KR_LOGWARNING, "Shutdown krcoordi...");
        kr_coordi_config_dump(&krcoordi, stdout);
        kr_coordi_finalize();
    }
    return 5000;
}

int kr_coordi_client_match(void *ptr, void *key)
{
    T_KRCoordiConnector *ptClient = (T_KRCoordiConnector *)ptr;
    char *clientid = (char *)key;
    
    return !strcmp(ptClient->id, clientid);
}


int kr_coordi_initialize(void)
{
    int ret = 0;
    
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    kr_coordi_sigterm_setup();
    
    /* Daemonize */
    if (krcoordi.daemonize) {
        kr_coordi_daemonize();
    }
    
    /* Set log file and level */
    kr_log_set_path(krcoordi.logpath);
    kr_log_set_name(krcoordi.coordiid);
    kr_log_set_level(krcoordi.loglevel);
    
    /* Construct consistent hash */
    krcoordi.fdtable = kr_hashtable_new(kr_int_hash, kr_int_equal);
    krcoordi.servers = kr_conhash_construct(NULL, NULL);
    krcoordi.clients = kr_list_new();
    kr_list_set_match(krcoordi.clients, (KRCompareFunc )kr_coordi_client_match);
        
    /* Create event loop */
    krcoordi.el = kr_event_loop_create(krcoordi.maxevents);

    /* Register coordi cron time event */
    kr_event_time_create(krcoordi.el, 1, kr_coordi_cron, NULL, NULL);

    if (krcoordi.tcpport != 0) {
        krcoordi.ipfd = kr_net_tcp_server(krcoordi.neterr, \
            krcoordi.tcpport, krcoordi.tcpbindaddr);
        if (krcoordi.ipfd == KR_NET_ERR) {
            KR_LOG(KR_LOGERROR, "kr_net_tcp_coordi[%d] failed[%s]",
                krcoordi.tcpport, krcoordi.neterr);
            return -1;
        }
    }
    if (krcoordi.ipfd < 0) {
        KR_LOG(KR_LOGERROR, "tcpport [%d] uncorrect!", krcoordi.tcpport);
        return -1;
    }
    
    /* Register tcp accept file event */
    if (krcoordi.ipfd > 0 && 
        kr_event_file_create(krcoordi.el, krcoordi.ipfd, KR_EVENT_READABLE, \
            kr_coordi_tcp_accept_handler, NULL) == KR_NET_ERR) 
    {
        KR_LOG(KR_LOGERROR, "kr_event_file_create tcp error");
        return -1;
    }
    
    return 0;
}


static int kr_coordi_finalize()
{
    /* event loop delete */
    kr_event_loop_delete(krcoordi.el);

    /* destroy fd's hashtable */
    kr_hashtable_destroy(krcoordi.fdtable);
    
    /* destruct server's consistent hash */
    kr_conhash_destruct(krcoordi.servers);
    
    /* destroy client's list*/
    kr_list_destroy(krcoordi.clients);
    
    /* Coordi config reset */
    kr_coordi_config_reset(&krcoordi);
    
    return 0;
}


int main(int argc, char *argv[])
{
    int ret = 0;
    
    /* Parse arguments to get the configure file name */
    ret = kr_coordi_argument_parse(argc, argv);
    if (ret != 0) {
        fprintf(stderr, "kr_coordi_argument_parse failed!\n");
        exit(1);
    }
    
    /* Parse configure file to the global structure */
    ret = kr_coordi_config_parse(krcoordi.configfile, &krcoordi);
    if (ret != 0) {
        fprintf(stderr, "kr_coordi_parse_configfile [%s] failed!\n", \
                krcoordi.configfile);
        exit(1);
    }
    
    /* initialize krcoordi */
    ret = kr_coordi_initialize();
    if (ret != 0) {
        fprintf(stderr, "kr_coordi_initialize failed!\n");
        exit(1);
    }

    /* Run the main event loop */
    kr_event_loop_run(krcoordi.el);
    
    exit(0);
}
