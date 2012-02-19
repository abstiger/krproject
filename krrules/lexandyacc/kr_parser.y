/****************************************************************************
kr_parser.y
krrules's parse file.
****************************************************************************/

%{
#include "kr_rules.h"
#include "kr_ruletree.h"

void yyerror();
%}

%pure_parser
%parse-param {KRParser *ruleparser}
%parse-param {void *scanner}
%lex-param {yyscan_t *scanner}

/*
%union{
    KRRuletree *ruletree;
}
%type <ruletree> line rule subrule term aggr regex primary
*/

%token SEMI ENDFILE ERROR 
%token ID NUM FNUM DID SID FID AID STR SCHAR REGEX
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
                { ruleparser->savedtree = $1; }
            ;
            
rule        :rule OR subrule 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = OR;
				}
			|rule AND subrule 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = AND;
				}
			|NOT rule
			    { $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $2;
				  $$ -> attr.op = NOT;
			    }
			|subrule
				{ $$ = $1; }
            ;
            
subrule		:subrule LT term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = LT;
				  $$ -> type = KR_BOOLEAN;
				}
			|subrule LE term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = LE;
				  $$ -> type = KR_BOOLEAN;
				}
			|subrule GT term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = GT;
				  $$ -> type = KR_BOOLEAN;
				}
			|subrule GE term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = GE;
				  $$ -> type = KR_BOOLEAN;
				}
			|subrule EQ term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = EQ;
				  $$ -> type = KR_BOOLEAN;
				}
			|subrule NEQ term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = NEQ;
				  $$ -> type = KR_BOOLEAN;
				}
			|subrule BL aggr 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = BL;
				  $$ -> type = KR_BOOLEAN;
				}
			|subrule NBL aggr 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = NBL;
				  $$ -> type = KR_BOOLEAN;
				}
			|subrule MATCH regex 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = MATCH;
				  $$ -> type = KR_BOOLEAN;
				}
			|term
				{ $$ = $1; }
            ;
            
term		:term PLUS term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = PLUS;
				}
			|term SUB term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = SUB;
				}
			|term MUT term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = MUT;
				}
			|term DIV term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = DIV;
				}
			|term MOD term 
				{ $$ = kr_ruletree_node_new(OpK);
				  $$ -> child[0] = $1;
				  $$ -> child[1] = $3;
				  $$ -> attr.op = MOD;
				}
			|primary
			    { $$ = $1; }
            ;

         
aggr        : AID
                { $$ = yylval;}
            ;
            
regex       : REGEX
                { $$ = yylval; }
            ;
            
primary     : NUM  { $$ = yylval; }
            | SUB NUM  %prec UMINUS 
              { $$ = yylval; $$->attr.val.i = -$$->attr.val.i;}
            | FNUM { $$ = yylval; }
            | SUB FNUM %prec UMINUS 
              { $$ = yylval; $$->attr.val.d = -$$->attr.val.d;}
            | STR  { $$ = yylval; }
            | SID  { $$ = yylval; }
            | DID  { $$ = yylval; }
			| LP rule RP
                { $$ = $2; }
			;
%%

KRParser *kr_parser_construct(char *rulestr)
{
    KRParser *pp = (KRParser *)zcalloc(sizeof(KRParser));
    pp->rulestr  = (char *)zstrdup(rulestr);

    yylex_init(&pp->scanner);
	yyset_extra(pp, pp->scanner);
	yy_scan_string(pp->rulestr, pp->scanner);
	yyparse(pp, pp->scanner);
    yylex_destroy(pp->scanner);
    return pp;
}

void kr_parser_destruct(KRParser *pp)
{
    if (pp != NULL) {
        zfree(pp->rulestr);
        kr_ruletree_free(pp->savedtree);
        zfree(pp); pp=NULL;
    }
}


void yyerror()
{
    
}
