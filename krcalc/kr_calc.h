#ifndef __KR_CALC_H__
#define __KR_CALC_H__

#include "krutils/kr_utils.h"
#include "kr_calc_parser.h"

/* callback function definition*/
typedef E_KRFieldType (*KRGetTypeFunc)(char kind, int id, void *param);
typedef void *(*KRGetValueFunc)(char kind, int id, void *param);

/*T_KRCalcTree forward declaration*/
typedef struct _kr_calc_tree_t T_KRCalcTree;

/* operation code */
typedef enum {
    /* arithmetic operation code */
    KR_CALCOP_PLUS   = 1,
    KR_CALCOP_SUB, 
    KR_CALCOP_MUT,
    KR_CALCOP_DIV,
    KR_CALCOP_MOD,
    /* logic operation code */
    KR_CALCOP_LT, 
    KR_CALCOP_LE, 
    KR_CALCOP_GT, 
    KR_CALCOP_GE, 
    KR_CALCOP_EQ, 
    KR_CALCOP_NEQ, 
    KR_CALCOP_BL, 
    KR_CALCOP_NBL, 
    KR_CALCOP_MATCH,
    KR_CALCOP_NOT, 
    KR_CALCOP_AND,
    KR_CALCOP_OR 
}E_KRCalcOp;

/* exp kind is used for node describing */
typedef enum {
    KR_CALCKIND_ARITH =1,   /*arithmetic expression*/
    KR_CALCKIND_LOGIC   ,   /*logical expression*/
    
    KR_CALCKIND_INT     ,   /*integer constant*/
    KR_CALCKIND_FLOAT   ,   /*float constant*/
    KR_CALCKIND_STRING  ,   /*string constant*/

    KR_CALCKIND_MINT    ,   /*multiple value*/
    KR_CALCKIND_MFLOAT  ,   /*multiple value*/
    KR_CALCKIND_MSTRING ,   /*multiple value*/
    KR_CALCKIND_REGEX   ,   /*regex expression*/

    KR_CALCKIND_SET     ,   /*set identifier*/
    KR_CALCKIND_CID     ,   /*current field identifier*/
    KR_CALCKIND_FID     ,   /*field identifier*/
    KR_CALCKIND_SID     ,   /*static identifier*/
    KR_CALCKIND_DID     ,   /*dynamic identifier*/
    KR_CALCKIND_HID         /*history identifier*/
}E_KRCalcKind;

/* Type is used for type checking */
typedef enum {
    KR_CALCTYPE_BOOLEAN    ='B',
    KR_CALCTYPE_INTEGER    ='I',
    KR_CALCTYPE_DOUBLE     ='D',
    KR_CALCTYPE_STRING     ='S',
    KR_CALCTYPE_UNKNOWN    ='U'
}E_KRCalcType;

/* Values is used for storing node value */
typedef union {
    boolean b;
    int     i;
    double  d;
    char   *s;
}U_KRCalcValue;

/* Format of this calculator */
typedef enum {
    KR_CALCFORMAT_FLEX = 'F', /* the Intuitive And Simple expression */
    KR_CALCFORMAT_JSON = 'J', /* json format expression */
    KR_CALCFORMAT_GWT  = 'G'  /* smartgwt filter's json expression */
}E_KRCalcFormat;


/* calculator definition */
typedef struct _kr_calc_t 
{
    /* outer params */
    E_KRCalcFormat    calc_format;
    char             *calc_string;
    KRGetTypeFunc     get_type_cb;
    KRGetValueFunc    get_value_cb;
    
    /* inner fields */
    T_KRCalcTree     *calc_tree;
    int               cid_cnt;
    int               fid_cnt;
    int               sid_cnt;
    int               did_cnt;
    int               hid_cnt;
    int               set_cnt;
    int               multi_cnt;
    int               regex_cnt;
    void             *lexer_state;     /* state of the lexer */
    int               parser_state;    /*0:success,-1:failure*/
    char              err_msg[200];
    void             *data;
    
    /* result fields */
    E_KRValueInd      result_ind;
    E_KRCalcType      result_type;
    U_KRCalcValue     result_value;
}T_KRCalc;


static inline E_KRCalcType kr_fieldtype_to_calctype(E_KRFieldType field_type)
{
    switch(field_type) 
    {
        case KR_FIELDTYPE_INT: return KR_CALCTYPE_INTEGER;
        case KR_FIELDTYPE_LONG: return KR_CALCTYPE_INTEGER;
        case KR_FIELDTYPE_DOUBLE: return KR_CALCTYPE_DOUBLE;
        case KR_FIELDTYPE_STRING: return KR_CALCTYPE_STRING;
        default: return KR_CALCTYPE_UNKNOWN;
    }
}

static inline E_KRFieldType kr_calctype_to_fieldtype(E_KRCalcType calc_type)
{
    switch(calc_type) 
    {
        case KR_CALCTYPE_BOOLEAN: return KR_FIELDTYPE_INT;
        case KR_CALCTYPE_INTEGER: return KR_FIELDTYPE_INT;
        case KR_CALCTYPE_DOUBLE: return KR_FIELDTYPE_DOUBLE;
        case KR_CALCTYPE_STRING: return KR_FIELDTYPE_STRING;
        default: return KR_FIELDTYPE_UNKNOWN;
    }
}


/* function declarations */
extern T_KRCalc *kr_calc_construct(E_KRCalcFormat format, char *calcstr, 
                   KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
extern void kr_calc_destruct(T_KRCalc *krcalc);
extern void kr_calc_dump(T_KRCalc *krcalc, FILE *printer);
extern int kr_calc_check(T_KRCalc *krcalc);
extern int kr_calc_eval(T_KRCalc *krcalc, void *param);


extern T_KRSet *kr_make_set_from_multi(E_KRCalcKind kind, char *string);
extern int kr_calc_parse(T_KRCalc *krcalc);

#endif    /* __KR_CALC_H__ */
