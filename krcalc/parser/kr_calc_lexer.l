/****************************************************************************
kr_calc_lexer.l
krcalc's Lex file.
****************************************************************************/

%{
#include "kr_calc.h"
#include "kr_calc_tree.h"
#include "kr_calc_parser.h"

T_KRSet *kr_make_set_from_multi(char *multi_str, E_KRCalcType calc_type);
%}

%option reentrant bison-bridge 
%option noyywrap nounput

/****************************************************************************
// declarations section
****************************************************************************/
digit       [0-9]
letter      [a-zA-Z]
letnum		[a-zA-Z0-9]
number      {digit}+
fnumber     {number}"."{number}
string       \'[^']*\'
currfldid   "C""_"{digit}+
fieldid     "F""_"{digit}+
staticid    "S""_"{digit}+
dynamicid   "D""_"{digit}+
historyid   "H""_"{digit}+
set         "A""_"{digit}+
num_ele     {number}","
fnum_ele    {fnumber}","
str_ele     {string}","
multi_num   "\{"{num_ele}+"\}"
multi_fnum  "\{"{fnum_ele}+"\}"
multi_str   "\{"{str_ele}+"\}"
regex       "\["[^"]*"\]"
constchar	'.'
newline		\n
whitespace  [ \t]+


/****************************************************************************
// rules section
****************************************************************************/
%%

"+"             {return PLUS;}
"-"             {return SUB;}
"*"             {return MUT;}
"/"             {return DIV;}
"%"             {return MOD;}
"<"             {return LT;}
"<="            {return LE;}
">"             {return GT;}
">="            {return GE;}
"=="            {return EQ;}
"!="            {return NEQ;}
"&&"            {return AND;}
"||"            {return OR;}
"="             {return ASSIGN;}
"!"             {return NOT;}
";"             {return SEMI;}
","             {return COMMA;}
"("             {return LP;}
")"             {return RP;}
"["             {return LSP;}
"]"             {return RSP;}
"{"             {return LFP;}
"}"             {return RFP;}
"@@"            {return BL;}
"!@"            {return NBL;}
"##"            {return MATCH;}
{number}        { (*yylval) = kr_calctree_node_new(KR_CALCKIND_INT);
			      (*yylval)->type = KR_CALCTYPE_INTEGER;
			      (*yylval)->attr.val.i = atoi(yytext);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return NUM;
			    }
{fnumber}       { (*yylval) = kr_calctree_node_new(KR_CALCKIND_FLOAT);
			      (*yylval)->type = KR_CALCTYPE_DOUBLE;
			      (*yylval)->attr.val.d = atof(yytext);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return FNUM;
			    }
{string}        { (*yylval) = kr_calctree_node_new(KR_CALCKIND_STRING);
			      (*yylval)->type = KR_CALCTYPE_STRING;
			      char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->attr.val.s = (char *)kr_strdup(caTemp);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return STR;
			    }
{constchar}		{ (*yylval) = kr_calctree_node_new(KR_CALCKIND_STRING);
			      (*yylval)->type = KR_CALCTYPE_STRING;
			      (*yylval)->attr.val.s = (char *)kr_strdup(yytext);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return SCHAR;
			    }
{currfldid}       { yyextra->cid_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_CID);
			      (*yylval)->name = (char *)kr_strdup(yytext);
			      return CID;
			    }
{fieldid}       { yyextra->fid_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_FID);
			      (*yylval)->name = (char *)kr_strdup(yytext);
			      return FID;
			    }
{staticid}      { yyextra->sid_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_SID);
			      (*yylval)->name = (char *)kr_strdup(yytext);
			      return SID;
			    }
{dynamicid}     { yyextra->did_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_DID);
			      (*yylval)->name = (char *)kr_strdup(yytext);
			      return DID;
			    }
{historyid}     { yyextra->hid_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_HID);
			      (*yylval)->name = (char *)kr_strdup(yytext);
			      return HID;
			    }
{set}           { yyextra->set_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_SET);
			      (*yylval)->name = (char *)kr_strdup(yytext);
			      E_KRFieldType ele_type = yyextra->get_type_cb((*yylval)->name, yyextra->data);
			      (*yylval)->type = kr_fieldtype_to_calctype(ele_type);
			      (*yylval)->attr.set = yyextra->get_value_cb((*yylval)->name, yyextra->data);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return SET;
			    }
{multi_num}     { yyextra->multi_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_MULTI);
			      (*yylval)->name = (char *)kr_strdup(yytext);
			      (*yylval)->type = KR_CALCTYPE_INTEGER;
			      (*yylval)->attr.set = kr_make_set_from_multi((*yylval)->name, (*yylval)->type);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return MUTLI;
                }
{multi_fnum}    { yyextra->multi_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_MULTI);
			      (*yylval)->name = (char *)kr_strdup(yytext);
			      (*yylval)->type = KR_CALCTYPE_DOUBLE;
			      (*yylval)->attr.set = kr_make_set_from_multi((*yylval)->name, (*yylval)->type);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return MUTLI;
                }
{multi_str}     { yyextra->multi_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_MULTI);
			      (*yylval)->name = (char *)kr_strdup(yytext);
			      (*yylval)->type = KR_CALCTYPE_STRING;
			      (*yylval)->attr.set = kr_make_set_from_multi((*yylval)->name, (*yylval)->type);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return MUTLI;
                }
{regex}         { yyextra->regex_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_REGEX);
                  char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->name = (char *)kr_strdup(caTemp);
			      (*yylval)->type = KR_CALCTYPE_STRING;
			      (*yylval)->attr.regex = kr_regex_compile((*yylval)->name);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return REGEX;
			    }
{newline}		{}
{whitespace}    {}

.               { ECHO; printf("error!~\n"); return ERROR;}


%%
/****************************************************************************
// programs section
****************************************************************************/

T_KRSet *kr_make_set_from_multi(char *multi_name, E_KRCalcType calc_type)
{
    T_KRSet *krset = NULL;
    char *multi_str = kr_strdup(multi_name+1); 
    multi_str[strlen(multi_str)-1]='\0';
    krset = kr_set_create(multi_str, kr_calctype_to_fieldtype(calc_type));
    
    char *save_str = NULL;
    char *p = strtok_r(multi_str, ",", &save_str);
    while(p)
    {
        switch(krset->type) 
        {
            case KR_FIELDTYPE_INT:
            {
                int i=atoi(p); 
                kr_set_add(krset, &i);
                break;
            }
            case KR_FIELDTYPE_LONG:
            {
                long l=atol(p);
                kr_set_add(krset, &l);
                break;
            }
            case KR_FIELDTYPE_DOUBLE:
            {
                double d=atof(p);
                kr_set_add(krset, &d);
                break;    
            }
            case KR_FIELDTYPE_STRING:
            {
                char *str=kr_strdup(p+1); 
                str[strlen(str)-1]='\0';
                kr_set_add(krset, str);
                kr_free(str);
                break;
            }
        }
        
        p=strtok_r(NULL, ",", &save_str);
    }
    kr_free(multi_str);
    
    return krset;
}
