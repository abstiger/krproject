#include "kr_data.h"

extern E_KRType kr_field_get_type(T_KRRecord *ptRecord, int fid);
extern void *kr_field_get_value(T_KRRecord *ptRecord, int fid);
extern E_KRType kr_set_get_type(int aid, T_KRData *ptData);
extern void *kr_set_get_value(int aid, T_KRData *ptData);
extern E_KRType kr_sdi_get_type(int sid, T_KRData *ptData);
extern void *kr_sdi_get_value(int sid, T_KRData *ptData);
extern E_KRType kr_ddi_get_type(int did, T_KRData *ptData);
extern void *kr_ddi_get_value(int did, T_KRData *ptData);
extern E_KRType kr_hdi_get_type(int hid, T_KRData *ptData);
extern void *kr_hdi_get_value(int hid, T_KRData *ptData);

E_KRType kr_data_get_type(char kind, int id, void *param)
{
    T_KRData *ptData = (T_KRData *)param;
    switch(kind) 
    {
        case KR_CALCKIND_CID:
            return kr_field_get_type(ptData->ptCurrRec, id);
            break;
        case KR_CALCKIND_FID:
            return kr_field_get_type(ptData->ptRecord, id);
            break;
        case KR_CALCKIND_SET:
            return kr_set_get_type(id, ptData);
            break;
        case KR_CALCKIND_SID:
            return kr_sdi_get_type(id, ptData);
            break;
        case KR_CALCKIND_DID: 
            return kr_ddi_get_type(id, ptData);
            break;
        case KR_CALCKIND_HID: 
            return kr_hdi_get_type(id, ptData);
            break;
        default:
            return KR_TYPE_UNKNOWN;
    }
}


void *kr_data_get_value(char kind, int id, void *param)
{
    T_KRData *ptData = (T_KRData *)param;
    switch(kind) 
    {
        case KR_CALCKIND_CID:
            return kr_field_get_value(ptData->ptCurrRec, id);
            break;
        case KR_CALCKIND_FID:
            return kr_field_get_value(ptData->ptRecord, id);
            break;
        case KR_CALCKIND_SET:
            return kr_set_get_value(id, ptData);
            break;
        case KR_CALCKIND_SID:
            return kr_sdi_get_value(id, ptData);
            break;
        case KR_CALCKIND_DID: 
            return kr_ddi_get_value(id, ptData);
            break;
        case KR_CALCKIND_HID: 
            return kr_hdi_get_value(id, ptData);
            break;
        default:
            return NULL;
    }
}

