#ifndef __KR_CALC_TREE_H__
#define __KR_CALC_TREE_H__

#include "kr_calc.h"

/* definitions for lexer and parser */
#define YYSTYPE         T_KRCalcTree*
#define YY_EXTRA_TYPE   T_KRCalc*

/* calculator tree definition */
struct _kr_calc_tree_t
{
    struct _kr_calc_tree_t **children;
    int                      childnum;
    
    E_KRCalcKind             kind;
    E_KRCalcOp               op;
    int                      id;

    E_KRType                 type;
    E_KRValueInd             ind;
    U_KRValue                value;
};

/* function declarations */
extern T_KRCalcTree *kr_calc_tree_new(E_KRCalcKind kind);
extern void kr_calc_tree_append(T_KRCalcTree *t, T_KRCalcTree *child);
extern void kr_calc_tree_free(T_KRCalcTree *root);

extern int kr_calc_tree_check(T_KRCalcTree *root, T_KRCalc *krcalc);
extern int kr_calc_tree_eval(T_KRCalcTree *root, T_KRCalc *krcalc);

#endif  /* __KR_CALC_TREE_H__ */
