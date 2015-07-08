#ifndef __KR_INPUT_HANDLE_H__
#define __KR_INPUT_HANDLE_H__

#include "krutils/kr_utils.h"

typedef void *(*KRInputPreFunc)(void *msg);
typedef void (*KRInputFunc)(void *fldval, int fldno, int fldlen, void *data);
typedef void (*KRInputPostFunc)(void *data);

typedef struct _kr_input_handle_t
{
    int              iInputId;
    char             caFormat[30+1];
    KRInputPreFunc   pfInputPre;
    KRInputFunc      pfInput;
    KRInputPostFunc  pfInputPost;
}T_KRInputHandle;


extern T_KRInputHandle* kr_input_handle_new(int iInputId, char *psFormat, T_KRModule *ptModule);
extern void kr_input_handle_free(T_KRInputHandle* ptInputHandle);
extern int kr_input_handle_match(T_KRInputHandle *ptInputHandle, char *psFormat);

#endif /* __KR_INPUT_HANDLE_H__ */
