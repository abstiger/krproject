#include "kr_data.h"

extern E_KRType kr_get_field_type(T_KRRecord *krrecord, int ifldid);
extern E_KRType kr_set_get_type(int aid, T_KRContext *krcontext);
extern E_KRType kr_sdi_get_type(int sid, T_KRContext *krcontext);
extern E_KRType kr_ddi_get_type(int did, T_KRContext *krcontext);
extern E_KRType kr_hdi_get_type(int hid, T_KRContext *krcontext);

extern void* kr_get_field_value(T_KRRecord *krrecord, int ifldid);
extern void *kr_set_get_value(int aid, T_KRContext *krcontext);
extern void *kr_sdi_get_value(int sid, T_KRContext *krcontext);
extern void *kr_ddi_get_value(int did, T_KRContext *krcontext);
extern void *kr_hdi_get_value(int hid, T_KRContext *krcontext);

E_KRType kr_data_get_type(char kind, int id, void *param)
{
    T_KRContext *ptContext = (T_KRContext *)param;
    switch(kind) 
    {
        case KR_CALCKIND_CID:
            return kr_get_field_type(ptContext->ptCurrRec, id);
            break;
        case KR_CALCKIND_FID:
            return kr_get_field_type(ptContext->ptRecord, id);
            break;
        case KR_CALCKIND_SET:
            return kr_set_get_type(id, ptContext);
            break;
        case KR_CALCKIND_SID:
            return kr_sdi_get_type(id, ptContext);
            break;
        case KR_CALCKIND_DID: 
            return kr_ddi_get_type(id, ptContext);
            break;
        case KR_CALCKIND_HID: 
            return kr_hdi_get_type(id, ptContext);
            break;
        default:
            return KR_TYPE_UNKNOWN;
    }
}


void *kr_data_get_value(char kind, int id, void *param)
{
    T_KRContext *ptContext = (T_KRContext *)param;
    switch(kind) 
    {
        case KR_CALCKIND_CID:
            return kr_get_field_value(ptContext->ptCurrRec, id);
            break;
        case KR_CALCKIND_FID:
            return kr_get_field_value(ptContext->ptRecord, id);
            break;
        case KR_CALCKIND_SET:
            return kr_set_get_value(id, ptContext);
            break;
        case KR_CALCKIND_SID:
            return kr_sdi_get_value(id, ptContext);
            break;
        case KR_CALCKIND_DID: 
            return kr_ddi_get_value(id, ptContext);
            break;
        case KR_CALCKIND_HID: 
            return kr_hdi_get_value(id, ptContext);
            break;
        default:
            return NULL;
    }
}

