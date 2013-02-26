#ifndef __KR_CALC_TREE_H__
#define __KR_CALC_TREE_H__

#include "kr_calc.h"

/* definitions for lexer and parser */
#define YYSTYPE         T_KRCalcTree*
#define YY_EXTRA_TYPE   T_KRCalc*

#define MAXCHILDREN 2
/* calculator tree definition */
struct _kr_calc_tree_t
{ 
    struct _kr_calc_tree_t *child[MAXCHILDREN];
    E_KRCalcOp              op;
    E_KRCalcKind            kind;
    int                     id;
    E_KRValueInd            ind;
    union { 
        U_KRCalcValue      val;
        T_KRSet           *set;
        T_KRRegex         *regex;
    }attr;
    E_KRCalcType            type;
};

/* function declarations */
extern T_KRCalcTree *kr_calctree_node_new(E_KRCalcKind kind);
extern int kr_calctree_check(T_KRCalcTree * root, char *errmsg);
extern int kr_calctree_eval(T_KRCalcTree * root, void *param);
extern void kr_calctree_dump(T_KRCalcTree *root, FILE *fp);
extern void kr_calctree_free(T_KRCalcTree *root);

#endif  /* __KR_CALC_TREE_H__ */
