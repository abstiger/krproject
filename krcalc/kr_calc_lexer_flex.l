/****************************************************************************
kr_calc_lexer.l
krcalc's Lex file.
****************************************************************************/

%{
#include "kr_calc.h"
#include "kr_calc_tree.h"
#include "kr_calc_parser_flex.h"
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
			      (*yylval)->id = atoi(yytext+2);
			      return CID;
			    }
{fieldid}       { yyextra->fid_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_FID);
			      (*yylval)->id = atoi(yytext+2);
			      return FID;
			    }
{staticid}      { yyextra->sid_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_SID);
			      (*yylval)->id = atoi(yytext+2);
			      return SID;
			    }
{dynamicid}     { yyextra->did_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_DID);
			      (*yylval)->id = atoi(yytext+2);
			      return DID;
			    }
{historyid}     { yyextra->hid_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_HID);
			      (*yylval)->id = atoi(yytext+2);
			      return HID;
			    }
{set}           { yyextra->set_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_SET);
			      (*yylval)->id = atoi(yytext+2);
			      (*yylval)->type = yyextra->get_type_cb(
                          KR_CALCKIND_SET, (*yylval)->id, yyextra->data);
			      (*yylval)->attr.set = yyextra->get_value_cb(
                          KR_CALCKIND_SET, (*yylval)->id, yyextra->data);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return SET;
			    }
{multi_num}     { yyextra->multi_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_MINT);
                  char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->type = KR_CALCTYPE_INTEGER;
			      (*yylval)->attr.set = \
                      kr_make_set_from_multi(KR_CALCKIND_MINT, caTemp);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return MULTI;
                }
{multi_fnum}    { yyextra->multi_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_MFLOAT);
                  char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->type = KR_CALCTYPE_DOUBLE;
			      (*yylval)->attr.set = \
                      kr_make_set_from_multi(KR_CALCKIND_MFLOAT, caTemp);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return MULTI;
                }
{multi_str}     { yyextra->multi_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_MSTRING);
                  char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->type = KR_CALCTYPE_STRING;
			      (*yylval)->attr.set = \
                      kr_make_set_from_multi(KR_CALCKIND_MSTRING, caTemp);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return MULTI;
                }
{regex}         { yyextra->regex_cnt++;
                  (*yylval) = kr_calctree_node_new(KR_CALCKIND_REGEX);
                  char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->type = KR_CALCTYPE_STRING;
			      (*yylval)->attr.regex = kr_regex_compile(caTemp);
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

