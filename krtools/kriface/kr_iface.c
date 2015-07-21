#include "kr_iface.h"
#include <sys/stat.h>

void *json_input_define_pre_func(T_KRParamInput *ptParamInput);
int json_input_define_func(T_KRParamInputField *ptParamInputField, void *data);
void json_input_define_post_func(void *data);

void *json_input_source_pre_func(T_KRParamInput *ptParamInput);
int json_input_source_func(T_KRParamInputField *ptParamInputField, void *data);
void json_input_source_post_func(void *data);

void *json_output_define_pre_func(T_KRParamOutput *ptParamOutput);
int json_output_define_func(T_KRParamOutputField *ptParamOutputField, void *data);
void json_output_define_post_func(void *data);

void *json_output_source_pre_func(T_KRParamOutput *ptParamOutput);
int json_output_source_func(T_KRParamOutputField *ptParamOutputField, void *data);
void json_output_source_post_func(void *data);



T_KRIfaceHandle gptIfaceHandle[]= 
{
    {
        "json", 
        json_input_define_pre_func, 
        json_input_define_func, 
        json_input_define_post_func,
        
        json_input_source_pre_func, 
        json_input_source_func, 
        json_input_source_post_func,

        json_output_define_pre_func, 
        json_output_define_func, 
        json_output_define_post_func,
                
        json_output_source_pre_func, 
        json_output_source_func, 
        json_output_source_post_func
    }
};

typedef struct _kr_iface_args_t
{
    int gen_define;
    int gen_source;
    int gen_makefile;
    
    char param_url[200];
    int  input_id;
    int  output_id;
    char format_name[20];
    char output_dir[1024];
}T_KRIfaceArgs;

static T_KRIfaceArgs gstArgs = {0};

static int kr_traversal_input_fields(T_KRParamInput *ptParamInput, 
    KRInputPreFunc pre_func, KRInputFunc func, KRInputPostFunc post_func, 
    void *priv)
{
    int iFlag = 0;
    
    /* run datasrc_field_pre_func */
    void *data = NULL;
    if (pre_func) {
        data = pre_func(ptParamInput);
    }
    
    for (int i=0; i<ptParamInput->lFieldCnt; ++i) {
        /* run datasrc_field_func */
        if (func) {
            T_KRParamInputField *ptFieldDef = &ptParamInput->ptFieldDef[i];
            iFlag = func(ptFieldDef, data);
            if (iFlag != 0) {
                fprintf(stderr, "run [%ld]:[%ld] func failed!\n", 
                        ptParamInput->lInputId, ptFieldDef->lFieldId);
                break;
            }
        }
    }
   
    /* run datasrc_field_post_func */
    if (post_func) {
        post_func(data);
    }

    return 0;
}

static int kr_traversal_output_fields(T_KRParamOutput *ptParamOutput, 
    KROutputPreFunc pre_func, KROutputFunc func, KROutputPostFunc post_func, 
    void *priv)
{
    int iFlag = 0;
    
    /* run datasrc_field_pre_func */
    void *data = NULL;
    if (pre_func) {
        data = pre_func(ptParamOutput);
    }
    
    for (int i=0; i<ptParamOutput->lFieldCnt; ++i) {
        /* run datasrc_field_func */
        if (func) {
            T_KRParamOutputField *ptFieldDef = &ptParamOutput->ptFieldDef[i];
            iFlag = func(ptFieldDef, data);
            if (iFlag != 0) {
                fprintf(stderr, "run [%ld]:[%ld] func failed!\n", 
                        ptParamOutput->lOutputId, ptFieldDef->lFieldId);
                break;
            }
        }
    }
   
    /* run datasrc_field_post_func */
    if (post_func) {
        post_func(data);
    }

    return 0;
}

static int _kr_traversal_input(char *psParamClassName, char *psParamObjectKey, 
            char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamInput *ptParamInput = (T_KRParamInput *)ptParamObject;

    /* Judge If datasrc matched */
    if (gstArgs.input_id != 0 && 
            gstArgs.input_id != ptParamInput->lInputId) {
        return 0;
    }

    for (int i=0; i<sizeof(gptIfaceHandle)/sizeof(T_KRIfaceHandle); ++i) {
        T_KRIfaceHandle *ptIfaceHandle = &gptIfaceHandle[i];

        /* Judge If format name matched */
        if (gstArgs.format_name[0] != '\0' &&
                strcasecmp(gstArgs.format_name, ptIfaceHandle->format_name)) {
            continue;
        }
                    
        /* generate define */
        if (gstArgs.gen_define) {
            int iFlag = kr_traversal_input_fields(ptParamInput, 
                    ptIfaceHandle->input_define_pre_func,
                    ptIfaceHandle->input_define_func,
                    ptIfaceHandle->input_define_post_func, data);
            if (iFlag != 0) {
                fprintf(stderr, "kr_traversal_input_fields [%ld] failed!\n", \
                        ptParamInput->lInputId);
                return -1;
            }
        }
        
        /* generate source */
        if (gstArgs.gen_source) {
            int iFlag = kr_traversal_input_fields(ptParamInput, 
                    ptIfaceHandle->input_source_pre_func,
                    ptIfaceHandle->input_source_func,
                    ptIfaceHandle->input_source_post_func, data);
            if (iFlag != 0) {
                fprintf(stderr, "kr_traversal_input_fields [%ld] failed!\n", \
                        ptParamInput->lInputId);
                return -1;
            }
        }
    }

    return 0;   
}

static int _kr_traversal_output(char *psParamClassName, char *psParamObjectKey, 
            char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamOutput *ptParamOutput = (T_KRParamOutput *)ptParamObject;

    /* Judge If datasrc matched */
    if (gstArgs.output_id != 0 && 
            gstArgs.output_id != ptParamOutput->lOutputId) {
        return 0;
    }

    for (int i=0; i<sizeof(gptIfaceHandle)/sizeof(T_KRIfaceHandle); ++i) {
        T_KRIfaceHandle *ptIfaceHandle = &gptIfaceHandle[i];

        /* Judge If format name matched */
        if (gstArgs.format_name[0] != '\0' &&
                strcasecmp(gstArgs.format_name, ptIfaceHandle->format_name)) {
            continue;
        }
                    
        /* generate define */
        if (gstArgs.gen_define) {
            int iFlag = kr_traversal_output_fields(ptParamOutput, 
                    ptIfaceHandle->output_define_pre_func,
                    ptIfaceHandle->output_define_func,
                    ptIfaceHandle->output_define_post_func, data);
            if (iFlag != 0) {
                fprintf(stderr, "kr_traversal_input_fields [%ld] failed!\n", \
                        ptParamOutput->lOutputId);
                return -1;
            }
        }
        
        /* generate source */
        if (gstArgs.gen_source) {
            int iFlag = kr_traversal_output_fields(ptParamOutput, 
                    ptIfaceHandle->output_source_pre_func,
                    ptIfaceHandle->output_source_func,
                    ptIfaceHandle->output_source_post_func, data);
            if (iFlag != 0) {
                fprintf(stderr, "kr_traversal_output_fields [%ld] failed!\n", \
                        ptParamOutput->lOutputId);
                return -1;
            }
        }
    }

    return 0;   
}

static int kr_traversal_input(T_KRParam *ptParam, void *data)
{
    //traversal input list
    if (kr_param_object_foreach(ptParam, KR_PARAM_INPUT, 
            _kr_traversal_input, data) != 0) {
        fprintf(stderr, "_kr_traversal_input error!");
        return -1;
    }

    return 0;
}

static int kr_traversal_output(T_KRParam *ptParam, void *data)
{
    //traversal output list
    if (kr_param_object_foreach(ptParam, KR_PARAM_OUTPUT, 
            _kr_traversal_output, data) != 0) {
        fprintf(stderr, "_kr_traversal_output error!");
        return -1;
    }

    return 0;
}

static int kr_generate_makefile(void)
{
    FILE *fp = fopen("Makefile", "w");
    if (fp == NULL) {
        fprintf(stdout, "open Makefile failed\n");
        return -1;
    }

    fprintf(fp, "MYLIB = libkriface.so\n");
    fprintf(fp, "SOURCES = $(wildcard *.c)\n");
    fprintf(fp, "OBJECTS = $(SOURCES:.c=.o)\n");

    fprintf(fp, "CC = gcc\n");
    fprintf(fp, "CFLAGS = -Wall -fPIC -DPIC -g -O2\n");
    fprintf(fp, "LDFLAGS = -shared\n");

    fprintf(fp, "\n");
    fprintf(fp, "CFLAGS += $(shell pkg-config --cflags jansson)\n");
    fprintf(fp, "LIBS += $(shell pkg-config --libs jansson)\n");

    fprintf(fp, "\n");
    fprintf(fp, "CFLAGS += $(shell pkg-config --cflags avro-c)\n");
    fprintf(fp, "LIBS += $(shell pkg-config --libs avro-c)\n");

    fprintf(fp, "\n");
    fprintf(fp, "all:$(MYLIB)\n");

    fprintf(fp, "\n");
    fprintf(fp, "%%.o:%%.c\n");
    fprintf(fp, "\t$(CC) $(CFLAGS) -c $<\n");

    fprintf(fp, "\n");
    fprintf(fp, "$(MYLIB):$(OBJECTS)\n");
    fprintf(fp, "\t$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@\n");
    fprintf(fp, "\t@echo \"build finished!\"\n");

    fprintf(fp, "\n");
    fprintf(fp, "install:$(MYLIB)\n");
    fprintf(fp, "\t@mv -f $(MYLIB) $(KRHOME)/lib\n");
    fprintf(fp, "\t@echo \"install finished!\"\n");

    fprintf(fp, "\n");
    fprintf(fp, "uninstall:\n");
    fprintf(fp, "\t@rm -f $(KRHOME)/lib/$(MYLIB)\n");
    fprintf(fp, "\t@echo \"uninstall finished!\"\n");

    fprintf(fp, "\n");
    fprintf(fp, "clean:\n");
    fprintf(fp, "\t@rm -f $(MYLIB) *.o\n");
    fprintf(fp, "\t@echo \"clean finished!\"\n");

    fprintf(fp, "\n");
    fprintf(fp, ".PHONY: all install uninstall clean");

    fclose(fp);

    return 0;
}

static void kr_usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: kriface [OPTIONS]\n"
            "  -p <param url>         Specify Param persist url(required) \n"
            "  -h                     Generate Define File(optional, default false) \n"
            "  -s                     Generate Source File(optional, default false) \n"
            "  -m                     Generate Make File(optional, default false) \n"
            "  -i <input id>          Specify Input Id(optional, default all) \n"
            "  -o <output id>         Specify Output Id(optional, default all) \n"
            "  -f <format name>       Specify Format Name(optional, default all) \n"
            "  -d <output dir>        Specify Output Directory(optional, default .) \n"
            "  -?                     Show this usage\n"
            "\n");
    return;
}


static int kr_parse_arguments(int argc, char *argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "hsmp:i:o:f:d:?")) != -1)
    {
        switch (opt)
        {
            case 'h':
                gstArgs.gen_define = 1;
                break;
            case 's':
                gstArgs.gen_source = 1;
                break;
            case 'm':
                gstArgs.gen_makefile = 1;
                break;
            case 'p':
                strcpy(gstArgs.param_url, optarg);
                break;    
            case 'i':
                gstArgs.input_id = atoi(optarg);
                break;
            case 'o':
                gstArgs.output_id = atoi(optarg);
                break;
            case 'f':
                strcpy(gstArgs.format_name, optarg);
                break;
            case 'd':
                strcpy(gstArgs.output_dir, optarg);
                break;
            default:
                kr_usage(argc, argv);
                return -1;
        }
    }

    return 0;
}


int main(int argc, char *argv[])
{
    int ret = -1;
    
    kr_log_set_path(".");
    kr_log_set_name("iface");
    kr_log_set_level(KR_LOGDEBUG);

    /* Parse Arguments */
    ret = kr_parse_arguments(argc, argv);
    if (ret != 0) {
        fprintf(stderr, "kr_parse_arguments failed!\n");
        return -1;
    }

    /* Change work directory */
    if (gstArgs.output_dir[0] != '\0') {
        if (access(gstArgs.output_dir, R_OK|W_OK|X_OK) != 0) {
            mkdir(gstArgs.output_dir, 0744);
        }
        if (chdir(gstArgs.output_dir) != 0) {
            fprintf(stderr, "chdir %s failed!\n", gstArgs.output_dir);
            return -1;
        }
    }

    if (gstArgs.param_url[0] == '\0') {
        fprintf(stderr, "param url should set!");
        return -1;
    }

    /* Create parameter memory */
    T_KRParam *ptParam = kr_param_create();
    if (ptParam == NULL) {
        fprintf(stderr, "kr_param_create failed!\n");
        goto clean;
    }

    /* load parameter */
    ret = kr_param_load(ptParam, gstArgs.param_url);
    if (ret < 0) {
        fprintf(stderr, "kr_param_load %s failed!\n", gstArgs.param_url);
        goto clean;
    }

    /* process input */
    ret = kr_traversal_input(ptParam, NULL);
    if (ret != 0) {
        fprintf(stderr, "kr_traversal_input failed!\n");
        goto clean;
    }

    /* process output */
    ret = kr_traversal_output(ptParam, NULL);
    if (ret != 0) {
        fprintf(stderr, "kr_traversal_output failed!\n");
        goto clean;
    }

    /* Create Makefile */
    if (gstArgs.gen_makefile) {
        ret = kr_generate_makefile();
        if (ret != 0) {
            fprintf(stderr, "kr_generate_makefile failed!\n");
            goto clean;
        }
    }

clean:
    if (ptParam) {
        kr_param_destroy(ptParam);
    }
    return ret;
}
