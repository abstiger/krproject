/****************************************************************************
kr_calc_parser.y
krcalc's parse file.
****************************************************************************/

%{
#include "kr_calc.h"
#include "kr_calc_tree.h"

extern int yylex();
#define YYERROR_VERBOSE 
void yyerror(T_KRCalc *krcalc, void *lexer_state, const char *errmsg);
%}

%pure_parser
%parse-param {T_KRCalc *krcalc}
%parse-param {void *scanner}
%lex-param {yyscan_t *scanner}

/*
%union{
    KRCalctree *tree;
}
%type <tree> line rule subrule term aggr regex primary
*/

%token SEMI ENDFILE ERROR 
%token ID NUM FNUM STR SCHAR CID FID SID DID HID SET MULTI REGEX
%left COMMA
%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left LT LE GT GE BL NBL
%left MATCH
%left PLUS SUB
%left MUT DIV MOD
%left LP RP LSP RSP LFP RFP
%right NOT
%nonassoc UMINUS


%start line

%% /* Grammar for AFD */

line        :rule SEMI
                { krcalc->calc_tree = $1; }
            ;
            
rule        :rule OR subrule 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_REL);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_OR;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|rule AND subrule 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_REL);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_AND;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|NOT rule
			    { $$ = kr_calc_tree_new(KR_CALCKIND_REL);
                  kr_calc_tree_append($$, $2);
				  $$ -> op = KR_CALCOP_NOT;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
			    }
			|subrule
				{ $$ = $1; }
            ;
            
subrule		:subrule LT term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_LOGIC);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_LT;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|subrule LE term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_LOGIC);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_LE;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|subrule GT term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_LOGIC);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_GT;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|subrule GE term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_LOGIC);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_GE;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|subrule EQ term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_LOGIC);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_EQ;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|subrule NEQ term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_LOGIC);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_NEQ;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|subrule BL aggr 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_LOGIC);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_BL;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|subrule NBL aggr 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_LOGIC);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_NBL;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|subrule MATCH regex 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_LOGIC);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_MATCH;
				  $$ -> type = KR_TYPE_BOOL;
				  $$ -> value.b = FALSE;
				}
			|term
				{ $$ = $1; }
            ;
            
term		:term PLUS term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_ARITH);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_PLUS;
				  $$ -> type = KR_TYPE_DOUBLE;
				  $$ -> value.d = 0.0;
				}
			|term SUB term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_ARITH);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_SUB;
				  $$ -> type = KR_TYPE_DOUBLE;
				  $$ -> value.d = 0.0;
				}
			|term MUT term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_ARITH);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_MUT;
				  $$ -> type = KR_TYPE_DOUBLE;
				  $$ -> value.d = 0.0;
				}
			|term DIV term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_ARITH);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_DIV;
				  $$ -> type = KR_TYPE_DOUBLE;
				  $$ -> value.d = 0.0;
				}
			|term MOD term 
				{ $$ = kr_calc_tree_new(KR_CALCKIND_ARITH);
                  kr_calc_tree_append($$, $1);
                  kr_calc_tree_append($$, $3);
				  $$ -> op = KR_CALCOP_MOD;
				  $$ -> type = KR_TYPE_INT;
				  $$ -> value.i = 0;
				}
			|primary
			    { $$ = $1; }
            ;

         
aggr        : SET
                { $$ = yylval;}
            | MULTI
                { $$ = yylval;}    
            ;
            
regex       : REGEX
                { $$ = yylval;}
            ;
            
primary     : NUM  { $$ = yylval;}
            | SUB NUM  %prec UMINUS 
              { $$ = yylval; 
                $$->value.i = -$$->value.i; 
              }
            | FNUM { $$ = yylval;}
            | SUB FNUM %prec UMINUS 
              { $$ = yylval; 
                $$->value.d = -$$->value.d;
              }
            | STR  { $$ = yylval;}
            | CID  { $$ = yylval;}
            | FID  { $$ = yylval;}
            | SID  { $$ = yylval;}
            | DID  { $$ = yylval;}
            | HID  { $$ = yylval;}
			| LP rule RP
                { $$ = $2; }
			;
%%
/****************************************************************************
// programs section
****************************************************************************/

void yyerror(T_KRCalc *krcalc, void *lexer_state, const char *errmsg)
{
    krcalc->calc_status = -1;
    memset(krcalc->calc_errmsg, 0x00, sizeof(krcalc->calc_errmsg));
    strcpy(krcalc->calc_errmsg, errmsg);
    fprintf(stderr, "parse error:%s\n", errmsg);
}
