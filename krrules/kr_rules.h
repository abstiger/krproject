#ifndef __KR_RULES_H__
#define __KR_RULES_H__

#include <stdio.h>

typedef struct _kr_parser  KRParser;
typedef struct _kr_rule  KRRule;

/* krrule's definition */
struct _kr_rule {
    char       *name;          /* name for this rule */
    KRParser   *parser;        /* parser for this rule */
    int        voilated;       /* 0--not voilated, 1--voilated; */
    void       *data;          /* stores whatever you needed */
};

extern KRRule *kr_rule_construct(char *name, char *rulestr, void *data);
extern int kr_rule_compute(KRRule *krrule, void *record, void *sta, void *dyn);
extern void kr_rule_print(KRRule *krrule, FILE *printer);
extern void kr_rule_destruct(KRRule *krrule);

#endif    /* __KR_RULES_H__ */
