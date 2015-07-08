#ifndef __KR_OUTPUT_HANDLE_H__
#define __KR_OUTPUT_HANDLE_H__

#include "kr_output.h"

typedef void *(*KROutputPreFunc)(void *msg);
typedef void (*KROutputFunc)(void *fldval, int fldno, int fldlen, void *data);
typedef void (*KROutputPostFunc)(void *data);

struct _kr_output_handle_t
{
    int              iTableId;
    char             caFormat[30+1];
    KROutputPreFunc   pfOutputPre;
    KROutputFunc      pfOutput;
    KROutputPostFunc  pfOutputPost;
};


extern T_KROutputHandle* kr_output_handle_new(int iTableId, char *psFormat, T_KRModule *ptModule);
extern void kr_output_handle_free(T_KROutputHandle* ptOutputHandle);
extern int kr_output_handle_match(T_KROutputHandle *ptOutputHandle, char *psFormat);
extern int kr_output_handle_process(T_KROutputHandle *ptOutputHandle, size_t size, void *buff, T_KRRecord *ptRecord);

#endif /* __KR_OUTPUT_HANDLE_H__ */
