#ifndef __KR_IFACE_INPUT_H__
#define __KR_IFACE_INPUT_H__

#include "krdb/kr_db.h"

typedef void *(*KRDBMapFuncPre)(void *msg);
typedef void (*KRDBMapFunc)(void *fldval, int fldno, int fldlen, void *data);
typedef void (*KRDBMapFuncPost)(void *data);

typedef struct _kr_input_handle_t
{
    int              iTableId;
    char             caFormat[30+1];
    KRDBMapFuncPre   pfMapPre;
    KRDBMapFunc      pfMap;
    KRDBMapFuncPost  pfMapPost;
}T_KRInputHandle;


extern T_KRInputHandle* kr_input_handle_new(int iTableId, char *psFormat, T_KRModule *ptModule);
extern void kr_input_handle_free(T_KRInputHandle* ptInputHandle);
extern int kr_input_handle_match(void *ptr, void *key);
extern int kr_input_handle_process(T_KRInputHandle *ptInputHandle, size_t size, void *buff, T_KRRecord *ptRecord);

#endif /* __KR_IFACE_INPUT_H__ */
