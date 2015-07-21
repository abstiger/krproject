#ifndef __KR_IFACE_H__
#define __KR_IFACE_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krparam/kr_param_class_input.h"
#include "krparam/kr_param_class_output.h"

typedef void *(*KRInputPreFunc)(T_KRParamInput *ptParamInput);
typedef int (*KRInputFunc)(T_KRParamInputField *ptParamInputField, void *data);
typedef void (*KRInputPostFunc)(void *data);

typedef void *(*KROutputPreFunc)(T_KRParamOutput *ptParamOutput);
typedef int (*KROutputFunc)(T_KRParamOutputField *ptParamOutputField, void *data);
typedef void (*KROutputPostFunc)(void *data);


typedef struct _kr_iface_handle_t
{
    char format_name[10];

    /*input define functions for this format*/
    KRInputPreFunc  input_define_pre_func;
    KRInputFunc     input_define_func;
    KRInputPostFunc input_define_post_func;
    
    /*input source functions for this format*/
    KRInputPreFunc  input_source_pre_func;
    KRInputFunc     input_source_func;
    KRInputPostFunc input_source_post_func;
    
    /*input define functions for this format*/
    KROutputPreFunc  output_define_pre_func;
    KROutputFunc     output_define_func;
    KROutputPostFunc output_define_post_func;
    
    /*output source functions for this format*/
    KROutputPreFunc  output_source_pre_func;
    KROutputFunc     output_source_func;
    KROutputPostFunc output_source_post_func;
}T_KRIfaceHandle;


#endif /* __KR_IFACE_H__ */
