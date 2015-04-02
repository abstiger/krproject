#include "kr_data.h"

T_KRData* kr_data_construct(T_KRParam *ptParam, 
        T_KRModule *ptModule, T_DbsEnv *ptDbsEnv, T_KRCache *ptHDICache,
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    short nSecId = ptParam->nSecId;
    
    T_KRData *ptData = kr_malloc(sizeof(T_KRData));
    if (ptData == NULL) {
        KR_LOG(KR_LOGERROR, "kr_malloc ptData Failed!");
        return NULL;
    }
    ptData->ptParam = ptParam;
    ptData->ptDbsEnv = ptDbsEnv;
    ptData->ptModule = ptModule;
    ptData->ptHDICache = ptHDICache;
    ptData->pfGetType = pfGetType;
    ptData->pfGetValue = pfGetValue;

    ptData->ptSetTable = kr_set_table_construct(&ptParam->stParamSet[nSecId], 
            ptDbsEnv);
    if (ptData->ptSetTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_set_table_construct Failed!");
        return NULL;
    }
    
    ptData->ptSdiTable = kr_sdi_table_construct(&ptParam->stParamSDI[nSecId], 
            ptModule, pfGetType, pfGetValue);
    if (ptData->ptSdiTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_sdi_table_construct Failed!");
        return NULL;
    }
    
    ptData->ptDdiTable = kr_ddi_table_construct(&ptParam->stParamDDI[nSecId], 
            ptModule, pfGetType, pfGetValue);
    if (ptData->ptDdiTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_ddi_table_construct Failed!");
        return NULL;
    }

    ptData->ptHdiTable = kr_hdi_table_construct(&ptParam->stParamHDI[nSecId], 
            ptModule);
    if (ptData->ptHdiTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hdi_table_construct Failed!");
        return NULL;
    }

    ptData->ptCurrRec = NULL;
    ptData->ptRecord = NULL;
    
    return ptData;
}

void kr_data_init(T_KRData *ptData)
{
    if (ptData) {
        kr_set_table_init(ptData->ptSetTable);
        kr_sdi_table_init(ptData->ptSdiTable);
        kr_ddi_table_init(ptData->ptDdiTable);
        kr_hdi_table_init(ptData->ptHdiTable);
    }
}

void kr_data_destruct(T_KRData *ptData)
{
    if (ptData) {
        kr_set_table_destruct(ptData->ptSetTable);
        kr_sdi_table_destruct(ptData->ptSdiTable);
        kr_ddi_table_destruct(ptData->ptDdiTable);
        kr_hdi_table_destruct(ptData->ptHdiTable);
        kr_free(ptData);
    }
}


int kr_data_check(T_KRData *ptData)
{  
    T_KRParam *ptParam = ptData->ptParam;
    short nSecId = ptData->ptParam->nSecId;
    T_KRParamSet *ptParamSet = &ptParam->stParamSet[nSecId];
    T_KRParamSDI *ptParamSDI = &ptParam->stParamSDI[nSecId];
    T_KRParamDDI *ptParamDDI = &ptParam->stParamDDI[nSecId];
    T_KRParamHDI *ptParamHDI = &ptParam->stParamHDI[nSecId];
    
    /*check set table*/
    if (ptData->ptSetTable->tConstructTime != ptParamSet->tLastLoadTime) {
        KR_LOG(KR_LOGDEBUG, "check reload kr_set_table_construct...[%ld][%ld]",
        ptData->ptSetTable->tConstructTime, ptParamSet->tLastLoadTime);
        T_KRSetTable *ptSetTable = kr_set_table_construct(ptParamSet, 
                ptData->ptDbsEnv);
        if (ptSetTable != NULL) {
            kr_set_table_destruct(ptData->ptSetTable);
            ptData->ptSetTable = ptSetTable;
        } else {
            KR_LOG(KR_LOGERROR, "reload set table error!");
            return -1;
        }
    }

    /*check sdi table*/
    if (ptData->ptSdiTable->tConstructTime != ptParamSDI->tLastLoadTime) {
        KR_LOG(KR_LOGDEBUG, "check reload kr_sdi_table_construct...");
        T_KRSDITable *ptSDITable = kr_sdi_table_construct(ptParamSDI, 
                ptData->ptModule, ptData->pfGetType, ptData->pfGetValue);
        if (ptSDITable != NULL) {
            kr_sdi_table_destruct(ptData->ptSdiTable);
            ptData->ptSdiTable = ptSDITable;
        } else {
            KR_LOG(KR_LOGERROR, "reload sdi table error!");
            return -1;
        }
    }
    
    /*check ddi table*/
    if (ptData->ptDdiTable->tConstructTime != ptParamDDI->tLastLoadTime) {
        KR_LOG(KR_LOGDEBUG, "check reload kr_ddi_table_construct...");
        T_KRDDITable *ptDDITable = kr_ddi_table_construct(ptParamDDI, 
                ptData->ptModule, ptData->pfGetType, ptData->pfGetValue);
        if (ptDDITable != NULL) {
            kr_ddi_table_destruct(ptData->ptDdiTable);
            ptData->ptDdiTable = ptDDITable;
        } else {
            KR_LOG(KR_LOGERROR, "reload ddi table error!");
            return -1;
        }
    }
    
    /*check hdi table*/
    if (ptData->ptHdiTable->tConstructTime != ptParamHDI->tLastLoadTime) {
        KR_LOG(KR_LOGDEBUG, "check reload kr_hdi_table_construct...");
        T_KRHDITable *ptHDITable = kr_hdi_table_construct(ptParamHDI, 
                ptData->ptModule);
        if (ptHDITable != NULL) {
            kr_hdi_table_destruct(ptData->ptHdiTable);
            ptData->ptHdiTable = ptHDITable;
        } else {
            KR_LOG(KR_LOGERROR, "reload hdi table error!");
            return -1;
        }
    }
    
    return 0;
}


