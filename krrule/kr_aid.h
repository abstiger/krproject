#ifndef __KR_AID_H__
#define __KR_AID_H__

#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"

T_KRSet *kr_aid_get_value(T_DbsEnv *dbsenv, int set_id);
E_KRFieldType kr_aid_get_type(T_DbsEnv *dbsenv, int set_id);

#endif  /*__KR_AID_H__*/
