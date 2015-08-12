#ifndef __KR_RESPONSE_H__
#define __KR_RESPONSE_H__

#include <stdio.h>

typedef struct _kr_response_t T_KRResponse;


/* function declaration */
T_KRResponse *kr_response_alloc(void);
void kr_response_free(T_KRResponse *ptResponse);

    
#endif /* __KR_RESPONSE_H__ */
