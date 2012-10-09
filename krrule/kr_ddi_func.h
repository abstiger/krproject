#ifndef __KR_DDI_FUNC_H__
#define __KR_DDI_FUNC_H__

typedef int  (*KRDDIAggrFunc)(void *p1, void *p2);

typedef enum {
    KR_DDI_STATISTICS_INCLUDE     = 'I',  /*include current record*/
    KR_DDI_STATISTICS_EXCLUDE     = 'X'   /*exclude current record*/
}E_KRDDIStatisticsType;

typedef enum {
    KR_DDI_METHOD_SUM        = '0',  /*sum*/
    KR_DDI_METHOD_MIN        = '1',  /*min*/
    KR_DDI_METHOD_MAX        = '2',  /*max*/
    KR_DDI_METHOD_COUNT      = '3',  /*count*/
    KR_DDI_METHOD_CON_INC    = '4',  /*continuous increase*/
    KR_DDI_METHOD_CON_DEC    = '5',  /*continuous decrease*/
    KR_DDI_METHOD_CNT_DIS    = '6'   /*count distinct*/
}E_KRDDIMethod;


#endif /* __KR_DDI_FUNC_H__ */
