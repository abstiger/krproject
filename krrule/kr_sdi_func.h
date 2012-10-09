#ifndef __KR_SDI_FUNC_H__
#define __KR_SDI_FUNC_H__

typedef int  (*KRSDIAggrFunc)(void *p1, void *p2);

typedef enum {
    KR_LOC_ABSOLUTE     = '0', /*绝对定位*/
    KR_LOC_RELATIVE     = '1'  /*相对定位*/
}E_KRLocProp;

#endif /* __KR_SDI_FUNC_H__ */
