#include "kr_server.h"
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

T_KRServer *krserver = NULL;

extern int kr_server_register_default(T_KRServer *krserver);

static int kr_server_startup(void);
static void kr_server_shutdown(void);


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
             krserver->config_file = kr_strdup(optarg);
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
    kr_server_shutdown();
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


static void kr_server_callback(T_KRMessage *apply, \
        T_KRMessage *reply, T_KRChannel *channel)
{
    
    kr_channel_send(channel, reply); 

}

static int kr_server_feed(T_KRChannel *channel, \
        T_KRMessage *apply, T_KRMessage *reply, T_KREngine *krengine)
{
    /* feed message to krengine */
    if (kr_engine_run(krengine, apply, reply, \
                (KRCBFunc )kr_server_callback, channel) != 0) {
        fprintf(stderr, "call kr_engine_run failed!\n");
        return -1;
    }
    
    return 0;
}


static int kr_server_startup(void)
{
    /* Parse configure file */
    krserver->config = kr_server_config_parse(krserver->config_file);
    if (krserver->config == NULL) {
        fprintf(stderr, "kr_server_config_parse [%s] failed!\n", \
                krserver->config_file);
        return -1;
    }

    /* signal handling */
    if (kr_server_signal_setup() != 0) {
        fprintf(stderr, "kr_server_signal_handle failed!\n");
        return -1;
    }
    
    /* Daemonize */
    if (krserver->config->daemonize) {
        kr_server_daemonize();
    }

    /* Create Pidfile */
    if (krserver->config->pidfile) {
        kr_server_create_pidfile(krserver->config->pidfile); 
    }

    /* Start up krengine */
    krserver->krengine = kr_engine_startup(krserver->config->engine, krserver);
    if (krserver->krengine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_engine_startup failed!\n");
        return -1;
    }

    /* Register server default handles */
    if (kr_server_register_default(krserver) != 0) {
        KR_LOG(KR_LOGERROR, "kr_server_register_default failed!\n");
        return -1;
    }
    
    /* Open channels */
    for (int i=0; i<krserver->config->channel_count; ++i) {
        T_KRChannelConfig *channel_config = krserver->config->channels[i];
        T_KRChannel *channel = krserver->krchannels[i];

        channel = kr_channel_open(channel_config, \
                (KRChannelFeedFunc )kr_server_feed, krserver->krengine);
        if (channel == NULL) {
            KR_LOG(KR_LOGERROR, "kr_channel_open %s failed!", \
                    channel_config->name);
            return -1;
        }    
        krserver->channel_count++; 
    }
    
    return 0;
}


static void kr_server_shutdown(void)
{
    /* Close channels */
    for (int i=0; i<krserver->channel_count; ++i) {
        kr_channel_close(krserver->krchannels[i]);
    }
    
    /* Shutdown krengine */
    kr_engine_shutdown(krserver->krengine);


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


int main(int argc, char *argv[])
{
    /* calloc krserver */
    krserver = kr_calloc(sizeof(*krserver));
    
    /* Parse arguments to get the configure file name */
    if (kr_server_argument_parse(argc, argv) != 0) {
        fprintf(stderr, "kr_server_argument_parse failed!\n");
        goto exit;
    }
    
    
    /* startup krserver */
    if (kr_server_startup() != 0) {
        fprintf(stderr, "kr_server_startup failed!\n");
        goto exit;
    }

    exit(0);

exit:
    kr_server_shutdown();
    exit(1);
}
