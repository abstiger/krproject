#ifndef __KR_RULETREE_H__
#define __KR_RULETREE_H__

#include "kr_rules.h"
#include "kr_regex.h"
#include "kr_dict.h"
#include "utils.h"

#define MAXCHILDREN 2

typedef struct _kr_ruletree KRRuletree;

struct _kr_ruletree
{ 
    struct _kr_ruletree *child[MAXCHILDREN];
    ExpKind expkind;
    char    *name;
    union { TokenType op;
            KRValue   val;
            KRDict   *krdict;
            KRRegex  *krregex;
          } attr;
    KRType type;
};

extern KRRuletree *kr_ruletree_node_new(ExpKind kind);
extern int kr_ruletree_node_free(KRRuletree *node, void *data);
extern int kr_ruletree_check(KRRuletree * syntaxTree);
extern void kr_ruletree_print(KRRuletree *root, FILE *printer);
extern void kr_ruletree_free(KRRuletree *root);

struct _kr_parser
{
    char            *rulestr;     /* input rule string */
    
    void            *scanner;     /* state of the lexer */
    int             fid_cnt;
    int             sid_cnt;
    int             did_cnt;
    int             aid_cnt;
    int             reg_cnt;
    
    KRRuletree        *savedtree; /* stores ruletree */
};

extern KRParser *kr_parser_construct(char *rulestr);
extern void kr_parser_destruct(KRParser *pp);

#define YYSTYPE         KRRuletree*
#define YY_EXTRA_TYPE   KRParser*


#endif  /* __KR_RULETREE_H__ */
