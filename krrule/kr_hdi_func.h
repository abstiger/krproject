#ifndef __KR_HDI_FUNC_H__
#define __KR_HDI_FUNC_H__

#define KR_HDI_OBJECT_LEN 30

typedef int (*KRHDIAggrFunc)(void *p1, void *p2);

typedef enum {
    KR_HDI_STATISTICS_DAY     = 'D',  /*day's statistics*/
    KR_HDI_STATISTICS_MONTH   = 'M',  /*month's statistics*/
    KR_HDI_STATISTICS_FLAG    = 'F'   /*flag's statistics*/
}E_KRHDIStatisticsType;

typedef enum {
    KR_HDI_METHOD_SUM        = '0',  /*sum*/
    KR_HDI_METHOD_MIN        = '1',  /*min*/
    KR_HDI_METHOD_MAX        = '2',  /*max*/
    KR_HDI_METHOD_COUNT      = '3',  /*count*/
    KR_HDI_METHOD_FLAG       = 'F'   /*flag*/
}E_KRHDIMethod;


#endif /* __KR_DDI_FUNC_H__ */
