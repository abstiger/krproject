#include "kr_server.h"
#include "kr_server_network.h"
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

char *config_file; /*configure file of krserver*/
T_KRServer *krserver=NULL;

extern int kr_server_register_default(T_KRServer *krserver);

static T_KRServer *kr_server_startup(char *config_file);
static void kr_server_serve(T_KRServer *krserver);
static void kr_server_shutdown(T_KRServer *krserver);


static void kr_server_usage(void)
{
    fprintf(stderr, "Usage: ./krserver -c[your configure file] [-h]\n");
}


static int kr_server_argument_parse(int argc, char *argv[])
{
    int opt;
    
    while ((opt = getopt(argc, argv, "c:h")) != -1)
    {
         switch(opt) {
         case 'c':
             config_file = kr_strdup(optarg);
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
    KR_LOG(KR_LOGWARNING, "Received Signal[%d], scheduling shutdown...", sig);
    fprintf(stderr, "Received Signal[%d], scheduling shutdown...\n", sig);
    kr_server_shutdown(krserver);
}


static int kr_server_signal_setup(void) 
{
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = kr_server_sigterm_action;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);

    return 0;
}


static void kr_server_remove_pidfile(char *pidfile) 
{
    unlink(pidfile);
}


static void kr_server_create_pidfile(char *pidfile) 
{
    /* Try to write the pid file in a best-effort way. */
    FILE *fp = fopen(pidfile, "w");
    if (fp) {
        fprintf(fp,"%d\n",(int)getpid());
        fclose(fp);
    }
}


static T_KRServer *kr_server_startup(char *config_file)
{
    /* calloc krserver */
    T_KRServer *krserver = kr_calloc(sizeof(*krserver));
    
    /* Parse configure file */
    krserver->config_file = config_file;
    krserver->config = kr_server_config_parse(krserver->config_file);
    if (krserver->config == NULL) {
        fprintf(stderr, "kr_server_config_parse [%s] failed!\n", \
                krserver->config_file);
        return NULL;
    }

    /* signal handling */
    if (kr_server_signal_setup() != 0) {
        fprintf(stderr, "kr_server_signal_handle failed!\n");
        return NULL;
    }
    
    /* Daemonize */
    if (krserver->config->daemonize) {
        kr_server_daemonize();
    }

    /* Startup krengine */
    krserver->krengine = kr_engine_startup(krserver->config->engine, krserver);
    if (krserver->krengine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_engine_startup failed!");
        return NULL;
    }

    /* Register server default handles */
    if (kr_server_register_default(krserver) != 0) {
        KR_LOG(KR_LOGERROR, "kr_server_register_default failed!");
        return NULL;
    }
    
    /* Create event loop */
    krserver->krel = kr_event_loop_create(1024);
    if (krserver->krel == NULL) {
        KR_LOG(KR_LOGERROR, "kr_event_loop_create failed!");
        return NULL;
    }

    /* Startup network */
    if (kr_server_network_startup(krserver) != 0) {
        KR_LOG(KR_LOGERROR, "kr_server_network_startup failed!");
        return NULL;
    }

    /* Create Pidfile */
    if (krserver->config->pidfile) {
        kr_server_create_pidfile(krserver->config->pidfile); 
    }
    
    return krserver;
}


static void kr_server_serve(T_KRServer *krserver)
{
    /* Run the main event loop */
    kr_event_loop_run(krserver->krel);

    /* event loop delete */
    kr_event_loop_delete(krserver->krel);
}


static void kr_server_shutdown(T_KRServer *krserver)
{
    if (krserver) {
        /* Shutdown network */
        kr_server_network_shutdown(krserver);

        /* Shutdown krengine */
        if (krserver->krengine) {
            kr_engine_shutdown(krserver->krengine);
        }

        /* event loop delete */
        if (krserver->krel) {
            kr_event_loop_stop(krserver->krel);
            //kr_event_loop_delete(krserver->krel);
        }

        /* Server config free */
        if (krserver->config) {
            /* remove pidfile */
            if (krserver->config->pidfile) {
                kr_server_remove_pidfile(krserver->config->pidfile);
            }
            kr_server_config_free(krserver->config);
        }

        /* free config file */
        if (krserver->config_file) {
            kr_free(krserver->config_file);
        }

        /* free server */
        kr_free(krserver);
    }
}


int main(int argc, char *argv[])
{
    /* Parse arguments to get the configure file */
    if (kr_server_argument_parse(argc, argv) != 0) {
        fprintf(stderr, "kr_server_argument_parse failed!\n");
        exit(1);
    }
    
    /* startup krserver */
    krserver = kr_server_startup(config_file);
    if (krserver == NULL) {
        fprintf(stderr, "kr_server_startup failed!\n");
        kr_server_shutdown(krserver);
        exit(1);
    }

    /* serve */
    kr_server_serve(krserver);

    exit(0);
}
