#ifndef __KR_INPUT_HANDLE_H__
#define __KR_INPUT_HANDLE_H__

#include "krutils/kr_utils.h"


typedef void *(*KRInputFuncPre)(void *msg);
typedef void (*KRInputFunc)(void *fldval, int fldno, int fldlen, void *data);
typedef void (*KRInputFuncPost)(void *data);

typedef struct _kr_input_handle_t
{
    int              iTableId;
    char             caFormat[30+1];
    KRInputFuncPre   pfInputPre;
    KRInputFunc      pfInput;
    KRInputFuncPost  pfInputPost;
}T_KRInputHandle;



extern T_KRInputHandle* kr_input_handle_new(int iTableId, char *psFormat, T_KRModule *ptModule);
extern void kr_input_handle_free(T_KRInputHandle* ptInputHandle);
extern int kr_input_handle_match(void *ptr, void *key);
extern int kr_input_handle_process(T_KRInputHandle *ptInputHandle, size_t size, void *buff, T_KRRecord *ptRecord);

#endif /* __KR_INPUT_HANDLE_H__ */
