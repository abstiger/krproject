/****************************************************************************
kr_lexer.l
krrules's Lex file.
****************************************************************************/

%{
#include "kr_rules.h"
#include "kr_ruletree.h"
#include "kr_parser.h"
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
string       \"[^"]*\"
dynamicid   "D""_"{digit}+
staticid    "S""_"{digit}+
fieldid     "F""_"{digit}+
aggrid      "A""_"{digit}+
regex       "["[^"]*"]"

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
{number}        { (*yylval) = kr_ruletree_node_new(NumK);
			      (*yylval)->type = KR_INTEGER;
			      (*yylval)->attr.val.i = atoi(yytext);
			      return NUM;
			    }
{fnumber}       { (*yylval) = kr_ruletree_node_new(FnumK);
			      (*yylval)->type = KR_DOUBLE;
			      (*yylval)->attr.val.d = atof(yytext);
			      return FNUM;
			    }
{string}        { (*yylval) = kr_ruletree_node_new(CharK);
			      (*yylval)->type = KR_STRING;
			      char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);
			      (*yylval)->attr.val.s = (char *)zstrdup(caTemp);
			      return STR;
			    }
{dynamicid}     { yyextra->did_cnt++;
                  (*yylval) = kr_ruletree_node_new(DidK);
			      (*yylval)->name = (char *)zstrdup(yytext);
			      return DID;
			    }
{staticid}      { yyextra->sid_cnt++;
                  (*yylval) = kr_ruletree_node_new(SidK);
			      (*yylval)->name = (char *)zstrdup(yytext);
			      return SID;
			    }
{fieldid}       { yyextra->fid_cnt++;
                  (*yylval) = kr_ruletree_node_new(FidK);
			      (*yylval)->name = (char *)zstrdup(yytext);
			      return FID;
			    }
{aggrid}        { yyextra->aid_cnt++;
                  (*yylval) = kr_ruletree_node_new(AggrK);
			      (*yylval)->name = (char *)zstrdup(yytext);
			      (*yylval)->attr.krdict = \
			          kr_dict_create((*yylval)->name, KR_STRING);
			      return AID;
			    }
{regex}         { yyextra->reg_cnt++;
                  (*yylval) = kr_ruletree_node_new(RegexK);
                  char caTemp[1024]={0};
			      strncpy(caTemp, yytext+1, yyleng -2);                  
			      (*yylval)->name = (char *)zstrdup(caTemp);
			      (*yylval)->attr.krregex = kr_regex_compile(caTemp);
			      return REGEX;
			    }
{constchar}		{ (*yylval) = kr_ruletree_node_new(CharK);
			      (*yylval)->type = KR_STRING;
			      (*yylval)->attr.val.s = (char *)zstrdup(yytext);
			      return SCHAR;
			    }
{newline}		{}
{whitespace}    {}

.               { ECHO; printf("error!~\n"); return ERROR;}


%%
/****************************************************************************
// programs section
****************************************************************************/

