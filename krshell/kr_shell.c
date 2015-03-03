#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include "kr_config.h"
#include "kr_client.h"

#ifdef HAVE_LIBREADLINE                                   
#  if defined(HAVE_READLINE_READLINE_H)                   
#    include <readline/readline.h>                        
#  elif defined(HAVE_READLINE_H)                          
#    include <readline.h>                                 
#  else /* !defined(HAVE_READLINE_H) */                   
/*static char *readline(char *prompt); */ 
#  endif /* !defined(HAVE_READLINE_H) */                  
char *cmdline = NULL;                                     
#else /* !defined(HAVE_READLINE_READLINE_H) */            
/* no readline */                                       
#endif /* HAVE_LIBREADLINE */                             

#ifdef HAVE_READLINE_HISTORY                              
#  if defined(HAVE_READLINE_HISTORY_H)                    
#    include <readline/history.h>                         
#  elif defined(HAVE_HISTORY_H)                           
#    include <history.h>                                  
#  else /* !defined(HAVE_HISTORY_H) */                    
# define add_history(X)   
# define read_history(X)  
# define write_history(X) 
# define stifle_history(X)
#  endif /* defined(HAVE_READLINE_HISTORY_H) */           
/* no history */                                        
#endif /* HAVE_READLINE_HISTORY */                        

static void kr_cmd_usage(int argc, char *argv[]);
static void kr_cmd_exit(int argc, char *argv[]);

static void kr_cmd_apply(T_KRMessage *apply);
static void kr_cmd_info(int argc, char *argv[]);
static void kr_cmd_info_log(int argc, char *argv[]);
static void kr_cmd_set_logpath(int argc, char *argv[]);
static void kr_cmd_set_logname(int argc, char *argv[]);
static void kr_cmd_set_loglevel(int argc, char *argv[]);

static void kr_cmd_info_krdb(int argc, char *argv[]);
static void kr_cmd_info_table(int argc, char *argv[]);
static void kr_cmd_info_index(int argc, char *argv[]);
static void kr_cmd_list_index_key(int argc, char *argv[]);

static void kr_cmd_reload_param(int argc, char *argv[]);
static void kr_cmd_info_param(int argc, char *argv[]);
static void kr_cmd_info_group(int argc, char *argv[]);
static void kr_cmd_info_group_rule(int argc, char *argv[]);
static void kr_cmd_info_set(int argc, char *argv[]);
static void kr_cmd_info_sdi(int argc, char *argv[]);
static void kr_cmd_info_ddi(int argc, char *argv[]);
static void kr_cmd_info_hdi(int argc, char *argv[]);

static void kr_cmd_insert_event(int argc, char *argv[]);
static void kr_cmd_detect_event(int argc, char *argv[]);
static void kr_cmd_insert_file(int argc, char *argv[]);
static void kr_cmd_detect_file(int argc, char *argv[]);

typedef void (*KRCommandFunc)(int argc, char *argv[]);

typedef struct _kr_cmd_t
{
    KRCommandFunc  cmd_func;
    int            argc;
    char          *argv[10];
}T_KRCmd;
static T_KRCmd krcmdmap[] = {
    {kr_cmd_usage,             1, {"?"}},
    {kr_cmd_usage,             1, {"H"}},
    {kr_cmd_usage,             1, {"HELP"}},
    {kr_cmd_exit,              1, {"Q"}},
    {kr_cmd_exit,              1, {"QUIT"}},
    {kr_cmd_exit,              1, {"EXIT"}},

    {kr_cmd_info,              1, {"INFO"}},
    {kr_cmd_info_log,          2, {"INFO", "LOG"}},
    {kr_cmd_set_logpath,       3, {"SET", "LOGPATH", "*"}},
    {kr_cmd_set_logname,       3, {"SET", "LOGNAME", "*"}},
    {kr_cmd_set_loglevel,      3, {"SET", "LOGLEVEL", "*"}},

    {kr_cmd_info_krdb,         2, {"INFO", "KRDB"}},
    {kr_cmd_info_table,        3, {"INFO", "TABLE", "*"}},
    {kr_cmd_info_index,        5, {"INFO", "TABLE", "*", "INDEX", "*"}},
    {kr_cmd_list_index_key,    2, {"LIST", "INDEX", "*", "KEY", "*"}},

    {kr_cmd_reload_param,      2, {"RELOAD", "PARAM"}},
    {kr_cmd_info_param,        2, {"INFO", "PARAM"}},
    {kr_cmd_info_group,        3, {"INFO", "GROUP", "*"}},
    {kr_cmd_info_group_rule,   5, {"INFO", "GROUP", "*", "RULE", "*"}},
    {kr_cmd_info_set,          3, {"INFO", "SET", "*"}},
    {kr_cmd_info_sdi,          3, {"INFO", "SDI", "*"}},
    {kr_cmd_info_ddi,          3, {"INFO", "DDI", "*"}},
    {kr_cmd_info_hdi,          3, {"INFO", "HDI", "*"}},

    {kr_cmd_insert_event,      4, {"INSERT", "*", "EVENT", "*"}},
    {kr_cmd_detect_event,      4, {"DETECT", "*", "EVENT", "*"}},
    {kr_cmd_insert_file,       4, {"INSERT", "*", "FILE", "*"}},
    {kr_cmd_detect_file,       4, {"DETECT", "*", "FILE", "*"}}
};

typedef struct _kr_ctx_t
{
    T_KRClient *krclient;
    char ip[32];
    int  port;
    char prompt[100];
    int  interactive;
    int  interrupted;
    int  history;
    char historyfile[1024];
    char outfile[1024];
    FILE *fdout;
    char errfile[1024];
    FILE *fderr;
    char errmsg[4098];
}T_KRCtx;
static T_KRCtx krctx = {0};


int kr_shell_init_ctx(void)
{
    if (krctx.ip[0] == '\0') strcpy(krctx.ip, "127.0.0.1");
    if (krctx.port == 0) krctx.port = 7251;
    krctx.interactive = 1;
    snprintf(krctx.prompt, sizeof(krctx.prompt), "krshell:-> "); 

    /*connect to krserver*/
    krctx.krclient = kr_client_connect(krctx.ip, krctx.port);
    if (krctx.krclient == NULL) {
        fprintf(stderr, "connect server [%s:%d] failed\n", 
                krctx.ip, krctx.port);
        return -1;
    }

    return 0;
}

void kr_shell_fini_ctx(void)
{
    kr_client_disconnect(krctx.krclient);
}


int kr_shell_parse_arguments(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "i:p:o:e:h")) != -1)
    {
        switch (opt)
        {
            case 'i':
                strcpy(krctx.ip, optarg);
                break;
            case 'p':
                krctx.port = atoi(optarg);
                break;
            case 'o':
                strcpy(krctx.outfile, optarg);
                break;
            case 'e':
                strcpy(krctx.errfile, optarg);
                break;
            default:
                kr_cmd_usage(argc, argv);
                exit(1);
        }
    }

    return 0;
}

void kr_shell_free_line(char *line)
{
    free(line);
}

char *kr_shell_read_line(FILE *in)
{
    char *line;

    if (in != NULL){
        /* FILE input */
        line = malloc(1024);
        if (fgets(line, 1024, in) == NULL) {
            free(line);
            return NULL;
        }
    } else {
#if defined(HAVE_LIBREADLINE) 
        line = readline(krctx.prompt);
#else
        printf("%s", krctx.prompt);
        fflush(stdout);
        line = malloc(1024);
        if (fgets(line, 1024, stdin) == NULL) {
            free(line);
            return NULL;
        }
        line[strlen(line)-1] = '\0';
#endif

#if defined(HAVE_READLINE_HISTORY) 
        if (line && *line) add_history(line);
#endif
    }

    return line;
}

void kr_shell_print_arg(int argc, char *argv[])
{
    fprintf(stdout, "argc:%d, ", argc);
    for (int i=0; i<argc; ++i) {
        fprintf(stdout, "argv%d:%s ", i, argv[i]);
    }
    fprintf(stdout, "\n");
}

void kr_shell_free_arg(int argc, char *argv[])
{
    int i;
    for (i=0; i<argc; ++i) {
        free(argv[i]);
    }
    free(argv);
}

int kr_shell_make_arg(char *line, int *pargc, char **pargv[])
{
    int argc = 0;
    char **argv = NULL;

    char *s = strdup(line);
    char *p = strtok(s, " ");
    while (p != NULL) {
        argv = realloc(argv, (argc+1)*sizeof(char *));
        argv[argc] = strdup(p);
        argc++;
        p = strtok(NULL, " ");
    }
    free(s);

    *pargc = argc;
    *pargv = argv;
    return 0;
}

T_KRCmd *kr_shell_search_command(int argc, char *argv[])
{
    T_KRCmd *krcmd = NULL;
    int nMatch;

    for (int i=0; i<sizeof(krcmdmap)/sizeof(T_KRCmd); ++i) {
        nMatch = 0;
        krcmd = &krcmdmap[i];
        if (argc != krcmd->argc) continue;
        for (int j=0; j<argc; ++j) {
            if (strcmp(krcmd->argv[j], "*") != 0) {
                if (strcasecmp(argv[j], krcmd->argv[j]) != 0) {
                    nMatch = -1;
                    break;
                }
            }
        }

        if (nMatch == 0) {
            return krcmd;
        } 
    }

    return NULL;
}

int kr_shell_do_command(int argc, char *argv[])
{
    /*search command*/
    T_KRCmd *krcmd = kr_shell_search_command(argc, argv);
    if (krcmd == NULL) {
        fprintf(stderr, "Command Not Found: ");
        kr_shell_print_arg(argc, argv);
        return 0;
    }
    
    /*do the command*/
    if (krcmd->cmd_func != NULL) {
        krcmd->cmd_func(argc, argv);
    }

    return 0;
}

int kr_shell_process_input(FILE *in)
{
    char *line = NULL;
    int argc = 0;
    char **argv = NULL;

    while(!krctx.interrupted) {
        /*read one line*/
        line = kr_shell_read_line(in);
        if (line == NULL) {
            fprintf(stderr, "fgets line failed!\n");
            return -1;
        }

        /*parse line into arguments*/
        if (kr_shell_make_arg(line, &argc, &argv) != 0) {
            fprintf(stderr, "kr_shell_make_arg [%s] failed!\n", line);
            kr_shell_free_line(line);
            return -1;
        }

        /*do the command*/
        if (kr_shell_do_command(argc, argv) != 0) {
            fprintf(stderr, "kr_shell_do_command [%s] failed!\n", line);
            kr_shell_free_arg(argc, argv);
            kr_shell_free_line(line);
            return -1;
        }

        /*free the parsed argumens*/
        kr_shell_free_arg(argc, argv);
        kr_shell_free_line(line);
    }

    return 0;
}

void kr_shell_signal_interrupt(int sig)
{
    krctx.interrupted = 1;
}

int main(int argc, char *argv[])
{  
    int ret = 0;

    signal(SIGINT, kr_shell_signal_interrupt);
    signal(SIGPIPE, SIG_IGN);

    /*parse arguments*/
    ret = kr_shell_parse_arguments(argc, argv);
    if (ret != 0) {
        fprintf(stderr, "kr_shell_parse_arguments failed!\n");
        return -1;
    }

    /*initialize context*/
    ret = kr_shell_init_ctx();
    if (ret != 0) {
        fprintf(stderr, "kr_shell_init_ctx failed!\n");
        return -1;
    }
    
    if (krctx.interactive == 0) {
        /*Run the command that follows krshell*/

    } else {
        /*Run commands received from standard input*/
        /*Use history when stdin is a tty*/
        if (isatty(fileno(stdin))) {
            krctx.history = 1;
            snprintf(krctx.historyfile, sizeof(krctx.historyfile), 
                    "%s/.krshell_history", getenv("HOME")); 
        }

#if defined(HAVE_READLINE_HISTORY) 
        if (krctx.history) {
            read_history(krctx.historyfile);
        }
#endif

        /*enter while loop, process user's input*/
        ret = kr_shell_process_input(NULL);
        if (ret != 0) {
            fprintf(stderr, "kr_shell_process_input failed[%s]\n", 
                    krctx.errmsg);
            goto CLEAN;
        }
    }
    
CLEAN:
    /*clean context*/
    kr_shell_fini_ctx();
    return ret;
}


static void kr_cmd_usage(int argc, char *argv[])
{
    fprintf(stderr,
            "Usage: krshell [OPTIONS]\n"
            "  -i <ip>                Server ip (default: 127.0.0.1)\n"
            "  -p <port>              Server port (default: 7251)\n"
            "  -o <output file>       Specified output file\n"
            "  -e <error file>        Specified error file\n"
            "  -h                     Show this usage\n"
            "Commands:\n"
            "  ?/H/HELP                            Show this usage\n"
            "  Q/QUIT/EXIT                         Exit shell\n"
               
            "  INFO                                Get engine info\n"
            "  INFO LOG                            Get log info\n"
            "  SET LOGPATH <path>                  Set log path\n"
            "  SET LOGNAME <name>                  Set log name\n"
            "  SET LOGLEVEL <level>                Set log level[1~5]\n"
               
            "  INFO KRDB                           Get krdb info\n"
            "  INFO TABLE <datasrcid>              Get datasrc info\n"
            "  INFO TABLE <id> INDEX <indexid>     Get datasrc index info\n"
            "  LIST INDEX <indexid> KEY <key>      Get index list with key\n"
               
            "  RELOAD PARAM                        Reload parameter\n"
            "  INFO PARAM                          Get parameter info\n"
            "  INFO GROUP <groupid>                Get group info\n"
            "  INFO GROUP <groupid> RULE <ruleid>  Get rule of group info\n"
            "  INFO SET <setid>                    Get set info\n"
            "  INFO SDI <sdiid>                    Get sdi info\n"
            "  INFO DDI <ddiid>                    Get ddi info\n"
            "  INFO HDI <hdiid>                    Get hdi info\n"
               
            "  INSERT <datasrcid> EVENT <event>    Insert event of datasrcid\n"
            "  DETECT <datasrcid> EVENT <event>    Detect event of datasrcid\n"
            "  INSERT <datasrcid> FILE <filename>  Insert file events of datasrcid\n"
            "  DETECT <datasrcid> FILE <filename>  Detect file events of datasrcid\n"
            "Examples:\n"
            "  DETECT 1 FILE test.dat      Send datasrc 1 file test.dat to detect\n"
            "\n");
    return;
}

static void kr_cmd_exit(int argc, char *argv[])
{
#if defined(HAVE_READLINE_HISTORY) 
        if (krctx.history) {
            stifle_history(50);
            write_history(krctx.historyfile);
        }
#endif
    fprintf(stdout, "exit\n");
    exit(1);
}

static void kr_cmd_print_reply(T_KRMessage *reply)
{
    if (reply == NULL) {
        fprintf(stderr, "kr_client_info failed!\n");
        return;
    } 
    
    /*print reply*/
    if (reply->msgtype == KR_MSGTYPE_SUCCESS) {
        fprintf(stdout, "SUCCESS: %s\n", reply->msgbuf);
    } else {
        fprintf(stdout, "ERROR!\n");
    }
    
    kr_message_free(reply);
}

static void kr_cmd_info(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info(krctx.krclient);
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_log(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_log(krctx.krclient);
    kr_cmd_print_reply(reply);
}

static void kr_cmd_set_logpath(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_set_logpath(krctx.krclient, argv[2]);
    kr_cmd_print_reply(reply);
}

static void kr_cmd_set_logname(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_set_logname(krctx.krclient, argv[2]);
    kr_cmd_print_reply(reply);
}

static void kr_cmd_set_loglevel(int argc, char *argv[])
{
    //check argv[2]
    if (atoi(argv[2]) < 1 || atoi(argv[2]) > 5) {
        fprintf(stderr, "invalid loglevel %s\n", argv[2]);
        return;
    }

    T_KRMessage *reply = kr_client_set_loglevel(krctx.krclient, atoi(argv[2]));
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_krdb(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_krdb(krctx.krclient);
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_table(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_table(krctx.krclient, atoi(argv[2]));
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_index(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_index(krctx.krclient, 
            atoi(argv[2]), atoi(argv[4]));
    kr_cmd_print_reply(reply);
}

static void kr_cmd_list_index_key(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_list_index_key(krctx.krclient);
    kr_cmd_print_reply(reply);
}

static void kr_cmd_reload_param(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_reload_param(krctx.krclient);
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_param(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_param(krctx.krclient);
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_group(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_group(krctx.krclient, atoi(argv[2]));
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_group_rule(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_group_rule(krctx.krclient, atoi(argv[2]), atoi(argv[4]));
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_set(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_set(krctx.krclient, atoi(argv[2]));
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_sdi(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_sdi(krctx.krclient, atoi(argv[2]));
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_ddi(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_ddi(krctx.krclient, atoi(argv[2]));
    kr_cmd_print_reply(reply);
}

static void kr_cmd_info_hdi(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_info_hdi(krctx.krclient, atoi(argv[2]));
    kr_cmd_print_reply(reply);
}

static void kr_cmd_insert_event(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_insert_event(krctx.krclient, atoi(argv[1]), argv[3]);
    kr_cmd_print_reply(reply);
}

static void kr_cmd_detect_event(int argc, char *argv[])
{
    T_KRMessage *reply = kr_client_detect_event(krctx.krclient, atoi(argv[1]), argv[3]);
    kr_cmd_print_reply(reply);
}

static void kr_cmd_insert_file(int argc, char *argv[])
{
    if (access(argv[3], R_OK) != 0) {
        fprintf(stderr, "file [%s] can not access!\n", argv[3]);
        return;
    }

    kr_client_insert_file(krctx.krclient, atoi(argv[1]), argv[3]);
}

static void kr_cmd_detect_file(int argc, char *argv[])
{
    if (access(argv[3], R_OK) != 0) {
        fprintf(stderr, "file [%s] can not access!\n", argv[3]);
        return;
    }

    kr_client_detect_file(krctx.krclient, atoi(argv[1]), argv[3]);
}

