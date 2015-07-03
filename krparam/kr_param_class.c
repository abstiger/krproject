#include "kr_param_class.h"
#include "kr_param_class_input.h"
#include "kr_param_class_output.h"
#include "kr_param_class_index.h"
#include "kr_param_class_set.h"
#include "kr_param_class_sdi.h"
#include "kr_param_class_ddi.h"
#include "kr_param_class_hdi.h"

#include "krutils/kr_utils.h"


T_KRParamClass gptParamClass[] = 
{
    //io
    {
        KR_PARAM_INPUT,
        (KRParamSerFunc )kr_param_input_serialize,
        (KRParamSerFreeFunc )kr_param_input_serialize_free,
        (KRParamDeserFunc )kr_param_input_deserialize,
        (KRParamDeserFreeFunc )kr_param_input_deserialize_free
    },
    {
        KR_PARAM_OUTPUT,
        (KRParamSerFunc )kr_param_output_serialize,
        (KRParamSerFreeFunc )kr_param_output_serialize_free,
        (KRParamDeserFunc )kr_param_output_deserialize,
        (KRParamDeserFreeFunc )kr_param_output_deserialize_free
    },
    
    //db
    {
        KR_PARAM_INDEX,
        (KRParamSerFunc )kr_param_index_serialize,
        (KRParamSerFreeFunc )kr_param_index_serialize_free,
        (KRParamDeserFunc )kr_param_index_deserialize,
        (KRParamDeserFreeFunc )kr_param_index_deserialize_free
    },
    
    //data
    {
        KR_PARAM_SET,
        (KRParamSerFunc )kr_param_set_serialize,
        (KRParamSerFreeFunc )kr_param_set_serialize_free,
        (KRParamDeserFunc )kr_param_set_deserialize,
        (KRParamDeserFreeFunc )kr_param_set_deserialize_free
    },
    {
        KR_PARAM_SDI,
        (KRParamSerFunc )kr_param_sdi_serialize,
        (KRParamSerFreeFunc )kr_param_sdi_serialize_free,
        (KRParamDeserFunc )kr_param_sdi_deserialize,
        (KRParamDeserFreeFunc )kr_param_sdi_deserialize_free
    },
    {
        KR_PARAM_DDI,
        (KRParamSerFunc )kr_param_ddi_serialize,
        (KRParamSerFreeFunc )kr_param_ddi_serialize_free,
        (KRParamDeserFunc )kr_param_ddi_deserialize,
        (KRParamDeserFreeFunc )kr_param_ddi_deserialize_free
    },
    {
        KR_PARAM_HDI,
        (KRParamSerFunc )kr_param_hdi_serialize,
        (KRParamSerFreeFunc )kr_param_hdi_serialize_free,
        (KRParamDeserFunc )kr_param_hdi_deserialize,
        (KRParamDeserFreeFunc )kr_param_hdi_deserialize_free
    }
    
    /*
    //flow
    {
        KR_PARAM_FLOW,
        (KRParamSerFunc )kr_param_flow_serialize,
        (KRParamSerFreeFunc )kr_param_flow_serialize_free,
        (KRParamDeserFunc )kr_param_flow_deserialize,
        (KRParamDeserFreeFunc )kr_param_flow_deserialize_free
    },
    {
        KR_PARAM_TASK,
        (KRParamSerFunc )kr_param_task_serialize,
        (KRParamSerFreeFunc )kr_param_task_serialize_free,
        (KRParamDeserFunc )kr_param_task_deserialize,
        (KRParamDeserFreeFunc )kr_param_task_deserialize_free
    }
    */
};


int kr_param_class_foreach(KRParamClassForEachFunc pfParamClassForEach, void *data)
{
    for (int i=0; i<sizeof(gptParamClass)/sizeof(T_KRParamClass); ++i) {
        T_KRParamClass *ptParamClass = &gptParamClass[i];
        if (pfParamClassForEach(ptParamClass, data) != 0) {
            return -1;
        }
    }
    return 0;
}


T_KRParamClass *kr_param_class_get(char *psParamClassName)
{
    for (int i=0; i<sizeof(gptParamClass)/sizeof(T_KRParamClass); ++i) {
        T_KRParamClass *ptParamClass = &gptParamClass[i];
        if (!strcmp(ptParamClass->psParamClassName, psParamClassName)) {
            return ptParamClass;
        }
    }
    return NULL;
}
