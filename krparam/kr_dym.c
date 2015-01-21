#include "kr_dym.h"

T_KRDynamicMem* kr_dym_construct(
        T_KRShareMem *ptShm, T_KRModule *ptDataModule, T_DbsEnv *dbsenv,
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func)
{
    short nSecId = ptShm->nSecId;
    
    T_KRDynamicMem *ptDyn = kr_malloc(sizeof(T_KRDynamicMem));
    if (ptDyn == NULL) {
        KR_LOG(KR_LOGERROR, "kr_malloc ptDyn Failed!");
        return NULL;
    }

    ptDyn->settable = kr_set_table_construct(&ptShm->stShmSet[nSecId], dbsenv);
    if (ptDyn->settable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_set_table_construct Failed!");
        return NULL;
    }
    
    ptDyn->sditable = kr_sdi_table_construct(&ptShm->stShmSDI[nSecId], 
            ptDataModule, get_type_func, get_value_func);
    if (ptDyn->sditable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_sdi_table_construct Failed!");
        return NULL;
    }
    
    ptDyn->dditable = kr_ddi_table_construct(&ptShm->stShmDDI[nSecId], 
            ptDataModule, get_type_func, get_value_func);
    if (ptDyn->dditable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_ddi_table_construct Failed!");
        return NULL;
    }

    ptDyn->hditable = kr_hdi_table_construct(&ptShm->stShmHDI[nSecId], 
            ptDataModule);
    if (ptDyn->hditable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hdi_table_construct Failed!");
        return NULL;
    }

    ptDyn->grouplist = kr_group_list_construct(&ptShm->stShmGroup[nSecId], 
            get_type_func, get_value_func);
    if (ptDyn->grouplist == NULL) {
        KR_LOG(KR_LOGERROR, "kr_group_list_construct Failed!");
        return NULL;
    }
    
    return ptDyn;
}

void kr_dym_init(T_KRDynamicMem *ptDyn)
{
    if (ptDyn) {
        kr_set_table_init(ptDyn->settable);
        kr_sdi_table_init(ptDyn->sditable);
        kr_ddi_table_init(ptDyn->dditable);
        kr_hdi_table_init(ptDyn->hditable);
        kr_group_list_init(ptDyn->grouplist);
    }
}

void kr_dym_destruct(T_KRDynamicMem *ptDyn)
{
    if (ptDyn) {
        kr_set_table_destruct(ptDyn->settable);
        kr_sdi_table_destruct(ptDyn->sditable);
        kr_ddi_table_destruct(ptDyn->dditable);
        kr_hdi_table_destruct(ptDyn->hditable);
        kr_group_list_destruct(ptDyn->grouplist);
        kr_free(ptDyn);
    }
}

cJSON *kr_dym_info(T_KRDynamicMem *ptDyn)
{ 
    cJSON *dym = cJSON_CreateObject();

    cJSON_AddNumberToObject(dym, "set_construct_time",
            (long )ptDyn->settable->tConstructTime);
    cJSON_AddNumberToObject(dym, "sdi_construct_time",
            (long )ptDyn->sditable->tConstructTime);
    cJSON_AddNumberToObject(dym, "ddi_construct_time",
            (long )ptDyn->dditable->tConstructTime);
    cJSON_AddNumberToObject(dym, "hdi_construct_time",
            (long )ptDyn->hditable->tConstructTime);
    cJSON_AddNumberToObject(dym, "group_construct_time",
            (long )ptDyn->grouplist->tConstructTime);

    return dym;
}

int kr_dym_check(T_KRDynamicMem *ptDyn, 
        T_KRShareMem *ptShm, T_KRModule *ptDataModule, T_DbsEnv *dbsenv,
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func)
{  
    short nShmSec = ptShm->nSecId;
    T_KRShmSet *ptShmSet = &ptShm->stShmSet[nShmSec];
    T_KRShmSDI *ptShmSDI = &ptShm->stShmSDI[nShmSec];
    T_KRShmDDI *ptShmDDI = &ptShm->stShmDDI[nShmSec];
    T_KRShmHDI *ptShmHDI = &ptShm->stShmHDI[nShmSec];
    T_KRShmGroup *ptShmGroup = &ptShm->stShmGroup[nShmSec];
    
    /*check set table*/
    if (ptDyn->settable->tConstructTime != ptShmSet->tLastLoadTime) {
        KR_LOG(KR_LOGDEBUG, "check reload kr_set_table_construct...[%ld][%ld]",
        ptDyn->settable->tConstructTime, ptShmSet->tLastLoadTime);
        T_KRSetTable *ptSetTable = kr_set_table_construct(ptShmSet, dbsenv);
        if (ptSetTable != NULL) {
            kr_set_table_destruct(ptDyn->settable);
            ptDyn->settable = ptSetTable;
        } else {
            KR_LOG(KR_LOGERROR, "reload set table error!");
            return -1;
        }
    }

    /*check sdi table*/
    if (ptDyn->sditable->tConstructTime != ptShmSDI->tLastLoadTime) {
        KR_LOG(KR_LOGDEBUG, "check reload kr_sdi_table_construct...");
        T_KRSDITable *ptSDITable = kr_sdi_table_construct(ptShmSDI, 
                ptDataModule, get_type_func, get_value_func);
        if (ptSDITable != NULL) {
            kr_sdi_table_destruct(ptDyn->sditable);
            ptDyn->sditable = ptSDITable;
        } else {
            KR_LOG(KR_LOGERROR, "reload sdi table error!");
            return -1;
        }
    }
    
    /*check ddi table*/
    if (ptDyn->dditable->tConstructTime != ptShmDDI->tLastLoadTime) {
        KR_LOG(KR_LOGDEBUG, "check reload kr_ddi_table_construct...");
        T_KRDDITable *ptDDITable = kr_ddi_table_construct(ptShmDDI, 
                ptDataModule, get_type_func, get_value_func);
        if (ptDDITable != NULL) {
            kr_ddi_table_destruct(ptDyn->dditable);
            ptDyn->dditable = ptDDITable;
        } else {
            KR_LOG(KR_LOGERROR, "reload ddi table error!");
            return -1;
        }
    }
    
    /*check hdi table*/
    if (ptDyn->hditable->tConstructTime != ptShmHDI->tLastLoadTime) {
        KR_LOG(KR_LOGDEBUG, "check reload kr_hdi_table_construct...");
        T_KRHDITable *ptHDITable = kr_hdi_table_construct(ptShmHDI, 
                ptDataModule);
        if (ptHDITable != NULL) {
            kr_hdi_table_destruct(ptDyn->hditable);
            ptDyn->hditable = ptHDITable;
        } else {
            KR_LOG(KR_LOGERROR, "reload hdi table error!");
            return -1;
        }
    }
    
    /*check group list*/
    if (ptDyn->grouplist->tConstructTime != ptShmGroup->tLastLoadTime) {
        KR_LOG(KR_LOGDEBUG, "check reload kr_group_list_construct...");
        T_KRGroupList *ptGroupList = kr_group_list_construct(ptShmGroup, 
                get_type_func, get_value_func);
        if (ptGroupList != NULL) {
            kr_group_list_destruct(ptDyn->grouplist);
            ptDyn->grouplist = ptGroupList;
        } else {
            KR_LOG(KR_LOGERROR, "reload group list error!");
            return -1;
        }
    }
    
    return 0;
}


