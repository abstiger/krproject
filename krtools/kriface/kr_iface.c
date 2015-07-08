#include "kr_iface.h"
#include <sys/stat.h>

T_KRIfaceFormat gptIfaceFormat[]= 
{
    {
        "fixed", "define",
        fixed_define_pre_func, 
        fixed_define_func, 
        fixed_define_post_func
    },
    {
        "fixed", "source",
        fixed_source_pre_func, 
        fixed_source_func, 
        fixed_source_post_func
    },
    {
        "json", "define",
        json_define_pre_func, 
        json_define_func, 
        json_define_post_func
    },
    {
        "json", "source",
        json_source_pre_func, 
        json_source_func, 
        json_source_post_func
    },
    {
        "avro", "define",
        avro_define_pre_func, 
        avro_define_func, 
        avro_define_post_func
    },
    {
        "avro", "source",
        avro_source_pre_func, 
        avro_source_func, 
        avro_source_post_func
    }
};

typedef struct _kr_iface_args_t
{
    int gen_define;
    int gen_source;
    int gen_makefile;
    int datasrc_id;
    char format_name[20];
    char output_path[1024];
}T_KRIfaceArgs;

static T_KRParam *gptParam = NULL;
static T_KRIfaceArgs gstArgs = {0};

static void kr_usage(int argc, char *argv[]);
static int kr_parse_arguments(int argc, char *argv[]);
static T_KRIfaceFormat *kr_search_format(char *format_name, char *file_code);
static int kr_traversal_datasrc(char *file_code);
static int kr_generate_makefile(void);


int main(int argc, char *argv[])
{
    int ret = -1;

    /* Parse Arguments */
    ret = kr_parse_arguments(argc, argv);
    if (ret != 0) {
        fprintf(stderr, "kr_parse_arguments failed!\n");
        return -1;
    }

    /* Create parameter memory */
    gptParam = kr_param_create();
    if (gptParam == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_create failed!");
        return -1;
    }
    //TODO: load parameter 

    /* generate define file */
    if (gstArgs.gen_define) {
        ret = kr_traversal_datasrc("define");
        if (ret != 0) {
            fprintf(stderr, "kr_traversal_datasrc define failed!\n");
            goto clean;
        }
    }

    /* generate source file */
    if (gstArgs.gen_source) {
        ret = kr_traversal_datasrc("source");
        if (ret != 0) {
            fprintf(stderr, "kr_traversal_datasrc source failed!\n");
            goto clean;
        }
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
    kr_param_destroy(gptParam);
    return ret;
}


static void kr_usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: kriface [OPTIONS]\n"
            "  -d                     Generate Define File \n"
            "  -s                     Generate Source File \n"
            "  -m                     Generate Make File \n"
            "  -i <datasrc id>        Specify Datasrc Id(default all) \n"
            "  -f <format name>       Specify Format Name(default all) \n"
            "  -o <output path>       Specify Output Directory(default .) \n"
            "  -h                     Show this usage\n"
            "\n");
    return;
}


static int kr_parse_arguments(int argc, char *argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "dsmi:f:o:h")) != -1)
    {
        switch (opt)
        {
            case 'd':
                gstArgs.gen_define = 1;
                break;
            case 's':
                gstArgs.gen_source = 1;
                break;
            case 'm':
                gstArgs.gen_makefile = 1;
                break;
            case 'i':
                gstArgs.datasrc_id = atoi(optarg);
                break;
            case 'f':
                strcpy(gstArgs.format_name, optarg);
                break;
            case 'o':
                strcpy(gstArgs.output_path, optarg);
                break;
            default:
                kr_usage(argc, argv);
                exit(1);
        }
    }

    /* Change work directory */
    if (gstArgs.output_path[0] != '\0') {
        if (access(gstArgs.output_path, R_OK|W_OK|X_OK) != 0) {
            mkdir(gstArgs.output_path, 0744);
        }
        if (chdir(gstArgs.output_path) != 0) {
            fprintf(stderr, "chdir %s failed!\n", gstArgs.output_path);
            return -1;
        }
    }

    return 0;
}


static T_KRIfaceFormat *kr_search_format(char *format_name, char *file_code)
{
    T_KRIfaceFormat *iface_format = NULL;

    for (int i=0; i<sizeof(gptIfaceFormat)/sizeof(T_KRIfaceFormat); ++i) {
        iface_format = &gptIfaceFormat[i];
        if (strcasecmp(format_name, iface_format->format_name) == 0 &&
            strcasecmp(file_code, iface_format->file_code) == 0) {
            return iface_format;
        }
    }
    return NULL;
}


static int _kr_input_define_new(char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamInput *ptParamInput = (T_KRParamInput *)ptParamObject;
    char *file_code = (char *)data;

    /* Judge If datasrc matched */
    if (gstArgs.datasrc_id != 0 && 
            gstArgs.datasrc_id != ptParamInput->lInputId) {
        return 0;
    }

    for (int i=0; i<sizeof(gptIfaceFormat)/sizeof(T_KRIfaceFormat); ++i) {
        T_KRIfaceFormat *iface_format = &gptIfaceFormat[i];
        /* Judge If file code matched */
        if (strcasecmp(file_code, iface_format->file_code) != 0) {
            return 0;
        }

        /* Judge If format name matched */
        if (gstArgs.format_name[0] != '\0' &&
                strcasecmp(gstArgs.format_name, iface_format->format_name)) {
            return 0;
        }

        /* Traversal Fields */
        int iFlag = kr_traversal_fields(ptParamInput, 
                iface_format->datasrc_field_pre_func,
                iface_format->datasrc_field_func,
                iface_format->datasrc_field_post_func);
        if (iFlag != 0) {
            fprintf(stderr, "kr_traversal_fields [%ld] failed!\n", \
                    ptParamInput->lInputId);
            return -1;
        }
    }

    return 0;   
}

static int kr_traversal_datasrc(char *file_code)
{
    /*create input list*/
    if (kr_param_object_foreach(gptParam, KR_PARAM_INPUT, 
            _kr_input_define_new, file_code) != 0) {
        KR_LOG(KR_LOGERROR, "_kr_db_build_table Error!");
        return -1;
    }

    return 0;
}


int kr_traversal_fields(T_KRParamInput *ptParamInput, 
        DatasrcFieldPreFunc pre_func, 
        DatasrcFieldFunc func, 
        DatasrcFieldPostFunc post_func)
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
            iFlag = func(&ptParamInput->ptFieldDef[i], data);
            if (iFlag != 0) {
                fprintf(stderr, "run [%ld] datasrc_field_func failed!\n", 
                        ptParamInput->lInputId);
                break;
            }
        }
    }
   
    /* run datasrc_field_post_func */
    if (post_func) {
        post_func(data, iFlag);
    }

    return iFlag;
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
