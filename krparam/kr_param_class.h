#ifndef __KR_PARAM_CLASS_H__
#define __KR_PARAM_CLASS_H__

typedef char* (*KRParamSerFunc)(void *ptParamObject);
typedef void  (*KRParamSerFreeFunc)(char *ptParamString);
typedef void*  (*KRParamDeserFunc)(char *psParamString);
typedef void  (*KRParamDeserFreeFunc)(void *ptParamObject);

/*parameter class definition*/
typedef struct _kr_param_class_t
{
    char                *psParamClassName;
    KRParamSerFunc       pfSerialize;
    KRParamSerFreeFunc   pfSerializeFree;
    KRParamDeserFunc     pfDeserialize;
    KRParamDeserFreeFunc pfDeserializeFree;
}T_KRParamClass;


typedef int (*KRParamClassForEachFunc)(T_KRParamClass *ptParamClass, void *data);
int kr_param_class_foreach(KRParamClassForEachFunc pfParamClassForEach, void *data);
T_KRParamClass *kr_param_class_get(char *psParamClassName);

#endif  /*__KR_PARAM_CLASS_H__*/
