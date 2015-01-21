#ifndef __KR_CALC_H__
#define __KR_CALC_H__

#include "krutils/kr_utils.h"

/* callback function definition*/
typedef E_KRType (*KRGetTypeFunc)(char kind, int id, void *param);
typedef void *(*KRGetValueFunc)(char kind, int id, void *param);

/*T_KRCalcTree forward declaration*/
typedef struct _kr_calc_tree_t T_KRCalcTree;

/* operation code */
typedef enum {
    /* arithmetic operation code */
    KR_CALCOP_PLUS   = 1,
    KR_CALCOP_SUB    = 2,    
    KR_CALCOP_MUT    = 3,
    KR_CALCOP_DIV    = 4,
    KR_CALCOP_MOD    = 5,
    /* logic operation code */
    KR_CALCOP_LT     = 6, 
    KR_CALCOP_LE     = 7, 
    KR_CALCOP_GT     = 8, 
    KR_CALCOP_GE     = 9, 
    KR_CALCOP_EQ     = 10, 
    KR_CALCOP_NEQ    = 11, 
    KR_CALCOP_BL     = 12, 
    KR_CALCOP_NBL    = 13, 
    KR_CALCOP_MATCH  = 14,
    /* relation operation code */
    KR_CALCOP_NOT    = 15, 
    KR_CALCOP_AND    = 16,
    KR_CALCOP_OR     = 17 
}E_KRCalcOp;

/* exp kind is used for node describing */
typedef enum {
    KR_CALCKIND_REL     = 0,   /*relation expression*/
    KR_CALCKIND_ARITH   = 1,   /*arithmetic expression*/
    KR_CALCKIND_LOGIC   = 2,   /*logical expression*/
    
    KR_CALCKIND_INT     = 3,   /*integer constant*/
    KR_CALCKIND_FLOAT   = 4,   /*float constant*/
    KR_CALCKIND_STRING  = 5,   /*string constant*/

    KR_CALCKIND_MINT    = 6,   /*multiple value*/
    KR_CALCKIND_MFLOAT  = 7,   /*multiple value*/
    KR_CALCKIND_MSTRING = 8,   /*multiple value*/
    KR_CALCKIND_REGEX   = 9,   /*regex expression*/

    KR_CALCKIND_SET     = 10,  /*set identifier*/
    KR_CALCKIND_CID     = 11,  /*current field identifier*/
    KR_CALCKIND_FID     = 12,  /*field identifier*/
    KR_CALCKIND_SID     = 13,  /*static identifier*/
    KR_CALCKIND_DID     = 14,  /*dynamic identifier*/
    KR_CALCKIND_HID     = 15   /*history identifier*/
}E_KRCalcKind;

/* Format of this calculator */
typedef enum {
    KR_CALCFORMAT_FLEX  = 'F', /* the Intuitive And Simple expression */
    KR_CALCFORMAT_JSON  = 'J', /* json format expression */
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
    void             *calc_state;        /*state of the lexer*/

    /* parameter for evaluate */
    void             *calc_param;
    int               calc_status;       /*0:success,-1:failure*/
    char              calc_errmsg[1024];
}T_KRCalc;

/* function declarations */
extern char *kr_calc_dump(T_KRCalc *krcalc, E_KRCalcFormat format);
extern int kr_calc_parse(T_KRCalc *krcalc);

extern T_KRCalc *kr_calc_construct(E_KRCalcFormat format, char *calcstr, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
extern void kr_calc_destruct(T_KRCalc *krcalc);
extern int kr_calc_check(T_KRCalc *krcalc);
extern int kr_calc_eval(T_KRCalc *krcalc, void *param);

/*calculator result functions*/
extern int kr_calc_status(T_KRCalc *krcalc);
extern char *kr_calc_errmsg(T_KRCalc *krcalc);
extern E_KRType kr_calc_type(T_KRCalc *krcalc);
extern U_KRValue *kr_calc_value(T_KRCalc *krcalc);
extern E_KRValueInd kr_calc_ind(T_KRCalc *krcalc);

#endif    /* __KR_CALC_H__ */
