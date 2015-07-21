#ifndef __KR_OUTPUT_HANDLE_H__
#define __KR_OUTPUT_HANDLE_H__

#include "krutils/kr_utils.h"

typedef void *(*KROutputPreFunc)(void);
typedef void (*KROutputFunc)(void *data, int fldno, int fldlen, void *fldval);
typedef void (*KROutputPostFunc)(void *data, size_t *msglen, char **msgbuf);

typedef struct _kr_output_handle_t
{
    int               iOutputId;
    char              caFormat[30+1];
    KROutputPreFunc   pfOutputPre;
    KROutputFunc      pfOutput;
    KROutputPostFunc  pfOutputPost;
}T_KROutputHandle;


extern T_KROutputHandle* kr_output_handle_new(int iTableId, char *psFormat, T_KRModule *ptModule);
extern void kr_output_handle_free(T_KROutputHandle* ptOutputHandle);
extern int kr_output_handle_match(T_KROutputHandle *ptOutputHandle, char *psFormat);

#endif /* __KR_OUTPUT_HANDLE_H__ */
