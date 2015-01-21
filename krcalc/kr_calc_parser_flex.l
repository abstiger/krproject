/****************************************************************************
kr_calc_lexer.l
krcalc's Lex file.
****************************************************************************/

%{
#include "kr_calc.h"
#include "kr_calc_tree.h"
#include "kr_calc_parser_flex.h"

extern int yyparse (T_KRCalc *krcalc, void *scanner);
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
{number}        { (*yylval) = kr_calc_tree_new(KR_CALCKIND_INT);
			      (*yylval)->type = KR_TYPE_INT;
			      (*yylval)->value.i = atoi(yytext);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return NUM;
			    }
{fnumber}       { (*yylval) = kr_calc_tree_new(KR_CALCKIND_FLOAT);
			      (*yylval)->type = KR_TYPE_DOUBLE;
			      (*yylval)->value.d = atof(yytext);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return FNUM;
			    }
{string}        { (*yylval) = kr_calc_tree_new(KR_CALCKIND_STRING);
			      (*yylval)->type = KR_TYPE_STRING;
			      char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->value.s = (char *)kr_strdup(caTemp);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return STR;
			    }
{constchar}		{ (*yylval) = kr_calc_tree_new(KR_CALCKIND_STRING);
			      (*yylval)->type = KR_TYPE_STRING;
			      (*yylval)->value.s = (char *)kr_strdup(yytext);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return SCHAR;
			    }
{currfldid}     { (*yylval) = kr_calc_tree_new(KR_CALCKIND_CID);
			      (*yylval)->id = atoi(yytext+2);
			      return CID;
			    }
{fieldid}       { (*yylval) = kr_calc_tree_new(KR_CALCKIND_FID);
			      (*yylval)->id = atoi(yytext+2);
			      return FID;
			    }
{staticid}      { (*yylval) = kr_calc_tree_new(KR_CALCKIND_SID);
			      (*yylval)->id = atoi(yytext+2);
			      return SID;
			    }
{dynamicid}     { (*yylval) = kr_calc_tree_new(KR_CALCKIND_DID);
			      (*yylval)->id = atoi(yytext+2);
			      return DID;
			    }
{historyid}     { (*yylval) = kr_calc_tree_new(KR_CALCKIND_HID);
			      (*yylval)->id = atoi(yytext+2);
			      return HID;
			    }
{set}           { (*yylval) = kr_calc_tree_new(KR_CALCKIND_SET);
			      (*yylval)->id = atoi(yytext+2);
			      return SET;
			    }
{multi_num}     { (*yylval) = kr_calc_tree_new(KR_CALCKIND_MINT);
                  char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->type = KR_TYPE_POINTER;
			      (*yylval)->value.p = kr_hashset_make(KR_TYPE_INT, caTemp);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return MULTI;
                }
{multi_fnum}    { (*yylval) = kr_calc_tree_new(KR_CALCKIND_MFLOAT);
                  char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->type = KR_TYPE_POINTER;
			      (*yylval)->value.p = kr_hashset_make(KR_TYPE_DOUBLE, caTemp);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return MULTI;
                }
{multi_str}     { (*yylval) = kr_calc_tree_new(KR_CALCKIND_MSTRING);
                  char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->type = KR_TYPE_POINTER;
			      (*yylval)->value.p = kr_hashset_make(KR_TYPE_STRING, caTemp);
			      (*yylval)->ind = KR_VALUE_SETED;
			      return MULTI;
                }
{regex}         { (*yylval) = kr_calc_tree_new(KR_CALCKIND_REGEX);
                  char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->type = KR_TYPE_POINTER;
			      (*yylval)->value.p = kr_regex_compile(caTemp);
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

int kr_calc_parse_flex(T_KRCalc *krcalc) 
{ 
    void *lexer_state;
    yylex_init(&lexer_state); 
    yyset_extra(krcalc, lexer_state); 
    yy_scan_string(krcalc->calc_string, lexer_state); 
    yyparse(krcalc, lexer_state); 
    yylex_destroy(lexer_state); 
    if (krcalc->calc_status == -1) { 
        return -1; 
    } 
    return 0; 
} 
