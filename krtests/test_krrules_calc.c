#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kr_rules.h"

int main(int argc, char *argv[])
{
    char rulestr[100] = {0};
    KRRule *myrule = NULL;
    
    if (argv[1] == NULL)
        strcpy(rulestr, "(1+4*27/9)&&(S_3 ## [^201111%])&&(D_1 @@ A_1);");
    else
        strcpy(rulestr, argv[1]);
    
    myrule = kr_rule_construct("rule_test", rulestr, NULL);
    
    kr_rule_print(myrule, stdout);
    
    kr_rule_evaluate(myrule, NULL, NULL, NULL);
    
    kr_rule_destruct(myrule);
    
    exit(0);
}
