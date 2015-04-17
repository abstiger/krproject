#ifndef __KR_IFACE_OUTPUT_H__
#define __KR_IFACE_OUTPUT_H__

#include "krdata/kr_data.h"

typedef void *(*KRDBMapFuncPre)(void *msg);
typedef void (*KRDBMapFunc)(void *fldval, int fldno, int fldlen, void *data);
typedef void (*KRDBMapFuncPost)(void *data);

typedef struct _kr_output_handle_t
{
    int              iTableId;
    char             caFormat[30+1];
    KRDBMapFuncPre   pfMapPre;
    KRDBMapFunc      pfMap;
    KRDBMapFuncPost  pfMapPost;
}T_KROutputHandle;


extern T_KROutputHandle* kr_output_handle_new(int iTableId, char *psFormat, T_KRModule *ptModule);
extern void kr_output_handle_free(T_KROutputHandle* ptOutputHandle);
extern int kr_output_handle_match(void *ptr, void *key);
extern int kr_output_handle_process(T_KROutputHandle *ptOutputHandle, size_t size, void *buff, T_KRRecord *ptRecord);

#endif /* __KR_IFACE_OUTPUT_H__ */
