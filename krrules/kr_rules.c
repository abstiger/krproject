#include <stdio.h>
#include <stdlib.h>
#include "kr_rules.h"
#include "kr_ruletree.h"
#include "zmalloc.h"

/* TODO:may need add some callback functions for "data"
 * but here we are not supposed to make it complicated
*/
KRRule *kr_rule_construct(char *name, char *rulestr, void *data)
{
    KRRule *krrule = zcalloc(sizeof(KRRule));
    krrule->name = zstrdup(name);
    krrule->parser = kr_parser_construct(rulestr);
    krrule->data = data;
    return krrule;
}


/* krrules are mainly considering three input parameters now:
 * record: where FIDs retrieve their values
 * sta:    where SIDs retrieve their values
 * dyn:    where DIDs retrieve there values
 *
 * TODO: make the input parameters alterable
*/
int kr_rule_evaluate(KRRule *krrule, void *record, void *sta, void *dyn)
{
    
    return 0;
}


/* print the rule to the printer */
void kr_rule_print(KRRule *krrule, FILE *printer)
{
    fprintf(printer, "\nPrint Rule:[%s]:\n    ", krrule->name);
    fprintf(printer, "fid_cnt:[%d],", krrule->parser->fid_cnt);
    fprintf(printer, "sid_cnt:[%d],", krrule->parser->sid_cnt);
    fprintf(printer, "did_cnt:[%d],", krrule->parser->did_cnt);
    fprintf(printer, "aid_cnt:[%d],", krrule->parser->aid_cnt);
    fprintf(printer, "reg_cnt:[%d]\n", krrule->parser->reg_cnt);
    kr_ruletree_print(krrule->parser->savedtree, printer);
    /* so we don't have data-print function now ... */
}


/* destruct krrule
 * NOTIFY: we do nothing about "data", destroy it yourself if needed
 */
void kr_rule_destruct(KRRule *krrule)
{
    if (krrule != NULL) {    
        if (krrule->name != NULL)
            zfree(krrule->name);
        if (krrule->parser != NULL)
            kr_parser_destruct(krrule->parser);    
        /* so we don't have data-destruct function now ... */    
        zfree(krrule); krrule = NULL;
    }
}