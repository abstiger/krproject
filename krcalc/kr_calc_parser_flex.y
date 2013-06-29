/****************************************************************************
kr_calc_parser.y
krcalc's parse file.
****************************************************************************/

%{
#include "kr_calc.h"
#include "kr_calc_tree.h"

#define YYERROR_VERBOSE 
void yyerror(T_KRCalc *krcalc, void *lexer_state, const char *errmsg);
%}

%pure_parser
%parse-param {T_KRCalc *krcalc}
%parse-param {void *scanner}
%lex-param {yyscan_t *scanner}

/*
%union{
    KRRuletree *ruletree;
}
%type <ruletree> line rule subrule term aggr regex primary
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
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_OR;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|rule AND subrule 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_AND;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|NOT rule
			    { $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $2;
				  $$ -> op = KR_CALCOP_NOT;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
			    }
			|subrule
				{ $$ = $1; }
            ;
            
subrule		:subrule LT term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_LT;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|subrule LE term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_LE;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|subrule GT term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_GT;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|subrule GE term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_GE;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|subrule EQ term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_EQ;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|subrule NEQ term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_NEQ;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|subrule BL aggr 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_BL;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|subrule NBL aggr 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_NBL;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|subrule MATCH regex 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_LOGIC);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_MATCH;
				  $$ -> type = KR_CALCTYPE_BOOLEAN;
				  $$ -> attr.val.b = FALSE;
				}
			|term
				{ $$ = $1; }
            ;
            
term		:term PLUS term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_ARITH);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_PLUS;
				  $$ -> type = KR_CALCTYPE_DOUBLE;
				  $$ -> attr.val.d = 0.0;
				}
			|term SUB term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_ARITH);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_SUB;
				  $$ -> type = KR_CALCTYPE_DOUBLE;
				  $$ -> attr.val.d = 0.0;
				}
			|term MUT term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_ARITH);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_MUT;
				  $$ -> type = KR_CALCTYPE_DOUBLE;
				  $$ -> attr.val.d = 0.0;
				}
			|term DIV term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_ARITH);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_DIV;
				  $$ -> type = KR_CALCTYPE_DOUBLE;
				  $$ -> attr.val.d = 0.0;
				}
			|term MOD term 
				{ $$ = kr_calctree_node_new(KR_CALCKIND_ARITH);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> op = KR_CALCOP_MOD;
				  $$ -> type = KR_CALCTYPE_INTEGER;
				  $$ -> attr.val.i = 0;
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
                $$->attr.val.i = -$$->attr.val.i; 
              }
            | FNUM { $$ = yylval;}
            | SUB FNUM %prec UMINUS 
              { $$ = yylval; 
                $$->attr.val.d = -$$->attr.val.d;
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

int kr_calc_parse_flex(T_KRCalc *krcalc)
{
    yylex_init(&krcalc->lexer_state);
	yyset_extra(krcalc, krcalc->lexer_state);
	yy_scan_string(krcalc->calc_string, krcalc->lexer_state);
	yyparse(krcalc, krcalc->lexer_state);
    yylex_destroy(krcalc->lexer_state);
    if (krcalc->parser_state == -1) {
	    return -1;
	}
	return 0;
}

void yyerror(T_KRCalc *krcalc, void *lexer_state, const char *errmsg)
{
    krcalc->parser_state = -1;
    memset(krcalc->err_msg, 0x00, sizeof(krcalc->err_msg));
    strcpy(krcalc->err_msg, errmsg);
    fprintf(stderr, "parse error:%s\n", errmsg);
}
