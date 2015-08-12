#ifndef __KR_REQUEST_H__
#define __KR_REQUEST_H__

#include <stdio.h>


typedef struct _kr_request_t T_KRRequest;


/* function declaration */
T_KRRequest *kr_request_alloc(void);
void kr_request_free(T_KRRequest *ptRequest);

    
#endif /* __KR_REQUEST_H__ */
