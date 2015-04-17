#ifndef __KR_IFACE_H__
#define __KR_IFACE_H__

#include "kr_io_input.h"
#include "kr_io_output.h"
#include "krengine/kr_engine.h"

typedef struct _kr_dict_value_t
{
    char           key[30+1];
    char           value[200+1];
}T_KRDictValue;

/*dictionary define*/
typedef struct _kr_dict_t
{
    int            id;
    char           name[30+1];
    char           desc[100+1];
    E_KRType       type;
    T_KRList       *list;
}T_KRDict;

/*input field define*/
typedef struct _kr_input_field_def_t
{
    int            id;
    char           name[30+1];
    char           desc[100+1];
    E_KRType       type;
    int            length;
    size_t         offset;
    T_KRDict       *dict;
}T_KRInputFieldDef;


typedef struct _kr_input_def_t
{
    int                iTableId;
    char               caTableName[30+1];
    char               caTableDesc[100+1];
    int                iFieldCnt;         /* field count of this table */
    T_KRInputFieldDef  *ptFieldDef;       /* field define of this table */
}T_KRInputDef;

/*output field define*/
typedef struct _kr_output_field_def_t
{
    int            id;
    char           name[30+1];
    char           desc[100+1];
    E_KRType       type;
    int            length;
    size_t         offset;
    T_KRCalc       *calc;
}T_KROutputFieldDef;

typedef struct _kr_output_def_t
{
    int                iTableId;
    char               caTableName[30+1];
    char               caTableDesc[100+1];
    int                iFieldCnt;         /* field count of this table */
    T_KROutputFieldDef *ptFieldDef;       /* field define of this table */
}T_KROutputDef;

typedef struct _kr_iface_t
{
    pthread_mutex_t  tLock;
    T_KRModule       *ptModule;
    
    T_KRList         *pInputDefList;
    T_KRList         *pOutputDefList;
         
    T_KRList         *pInputHandleList;
    T_KRList         *pOutputHandleList;
}T_KRIface;

T_KRIface *kr_iface_create(T_KRModule *ptModule, T_DbsEnv *ptDbsEnv); 
void kr_iface_destroy(T_KRIface *ptIface);

int kr_iface_process_input(T_KRIface *ptIface, T_KRMessage *ptInput, T_KRRecord *ptRecord);
int kr_iface_generate_output(T_KRIface *ptIface, T_KRContext *ptContext, T_KRMessage *ptOutput);

#endif /* __KR_IFACE_H__ */
