#include <stdio.h>
#include <stdlib.h>
#include "kr_ruletree.h"
#include "kr_parser.h"
#include "zmalloc.h"

/* ruletree traversal function definition */
typedef int (*traverse_func)(KRRuletree *node, void *data);

/* variable indentno is used by kr_ruletree_node_print to
 * store current number of spaces to indent
 */
static indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* create a new ruletree node */
KRRuletree *kr_ruletree_node_new(ExpKind kind)
{ 
    int i;  
    KRRuletree *t = (KRRuletree *)zcalloc(sizeof(KRRuletree));
    if (t == NULL) {
        fprintf(stderr, "zmalloc KRRuletree failed!\n");
        return NULL;
    }
    t->name = NULL;
    t->expkind = kind;
    for (i=0; i<MAXCHILDREN; i++) {
        t->child[i] = NULL;
    }
    return t;
}


/* free this ruletree node and free his allocated subvalue */
int kr_ruletree_node_free(KRRuletree *node, void *data)
{ 
    if (node != NULL) {
        if (node->name) zfree(node->name);
        switch (node->expkind)
        {
            case CharK:  zfree(node->attr.val.s); break;
            case AggrK:  kr_dict_release(node->attr.krdict); break;
            case RegexK: kr_regex_free(node->attr.krregex); break;
        }
        zfree(node); node = NULL;
    }
    return KR_SUCCESS;
}

/* print operator token, refer to the lexer file's definition
 */
static void kr_ruletree_node_print_opr(FILE *printer, TokenType token)
{ 
    switch (token)
    {
        case PLUS: fprintf(printer,"+\n"); break;
        case SUB: fprintf(printer,"-\n"); break;
        case MUT: fprintf(printer,"*\n"); break;
        case DIV: fprintf(printer,"/\n"); break;
        case LT: fprintf(printer,"<\n"); break;
        case LE: fprintf(printer,"<=\n"); break;
        case GT: fprintf(printer,">\n"); break;
        case GE: fprintf(printer,">=\n"); break;
        case EQ: fprintf(printer,"==\n"); break;
        case NEQ: fprintf(printer,"!=\n"); break;
        case BL: fprintf(printer,"@@\n"); break;
        case NBL: fprintf(printer,"!@\n"); break;
        case AND: fprintf(printer,"&&\n"); break;
        case OR: fprintf(printer,"||\n"); break;
        case NOT: fprintf(printer,"!\n"); break;
        case ASSIGN: fprintf(printer,"=\n"); break;
        case SEMI: fprintf(printer,";\n"); break;
        case COMMA: fprintf(printer,",\n"); break;
        case LP: fprintf(printer,"(\n"); break;
        case RP: fprintf(printer,")\n"); break;
        case LSP: fprintf(printer,"[\n"); break;
        case RSP: fprintf(printer,"]\n"); break;
        case LFP: fprintf(printer,"{\n"); break;
        case RFP: fprintf(printer,"}\n"); break;
        case MATCH: fprintf(printer,"##\n"); break;
        default: /* should never happen */
            fprintf(printer,"Unknown token: %d\n",token);
    }
}

/* preprocess of printing ruletree node  */
static int kr_ruletree_node_print_pre(KRRuletree *node, void *data)
{ 
    int i = 0;
    FILE *printer = data;
    
    INDENT;
    for (i=0; i<indentno; i++)
        fprintf(printer, " ");

    switch(node->expkind){
        case OpK:
            fprintf(printer,"Op: ");
            kr_ruletree_node_print_opr(printer, node->attr.op);
            break;
        case NumK:
            fprintf(printer,"Num_Const: %d\n",node->attr.val.i);
            break;
        case FnumK:
            fprintf(printer,"FNum_Const: %f\n",node->attr.val.d);
            break;
        case CharK:
            fprintf(printer,"Char_Const: %s\n",node->attr.val.s);
            break;
        case FidK:
            fprintf(printer,"Fid: %s\n",node->name);
            break;
        case SidK:
            fprintf(printer,"Sid: %s\n",node->name);
            break;
        case DidK:
            fprintf(printer,"Did: %s\n",node->name);
            break;    
        case AggrK:
            fprintf(printer,"Aggr: %s\n",node->name);
            break;
        case RegexK:
            fprintf(printer,"Regex: %s\n",node->name);
            break;    
        default:
            fprintf(printer,"Unknown expkind: %d\n",node->expkind);
            return KR_FAILURE;
    }
    return KR_SUCCESS;
}


/* postprocess of printing ruletree node  */
static int kr_ruletree_node_print_post(KRRuletree *node, void *data)
{ 
    UNINDENT;
    return KR_SUCCESS;
}

/* procedure kr_ruletree_traverse is a generic recursive
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc in postorder
 * to tree pointed to by t
 */
static int kr_ruletree_traverse(KRRuletree * t, void *data,
                     traverse_func preProc, traverse_func postProc)
{ 
    if (t != NULL) { 
        int i;
        if (preProc != NULL) {
            if (preProc(t, data) != KR_SUCCESS) return KR_FAILURE;
        }
        for (i=0; i < MAXCHILDREN; i++)
            kr_ruletree_traverse(t->child[i], data, preProc, postProc);
        if (postProc != NULL) {
            if (postProc(t, data) != KR_SUCCESS) return KR_FAILURE;
        }
    }
}

/* procedure checkNode performs
 * type checking at a single tree node
 */
static int checkNode(KRRuletree * t, void *data)
{ 
    switch (t->expkind)
    { 
    case OpK:
        switch(t->attr.op) {
        case PLUS: case SUB: case MUT: case DIV: case MOD:
            if ((t->child[0]->type != KR_INTEGER && t->child[0]->type != KR_DOUBLE) ||
                (t->child[1]->type != KR_INTEGER && t->child[1]->type != KR_DOUBLE)) {
                fprintf(stderr,"Type error:%s\n", "Op applied to non-number");
                return KR_FAILURE;
            }
            else if (t->child[0]->type == KR_DOUBLE || t->child[1]->type == KR_DOUBLE)
                t->type = KR_DOUBLE;
            else
                t->type = KR_INTEGER;
            break;
        case LT: case LE: case GT: case GE: case EQ: case NEQ:
            if ((t->child[0]->type != KR_INTEGER && t->child[0]->type != KR_DOUBLE) ||
                (t->child[1]->type != KR_INTEGER && t->child[1]->type != KR_DOUBLE)) {
                fprintf(stderr,"Type error:%s\n", "Op applied to non-number");
                return KR_FAILURE;
            }
            else 
                t->type = KR_BOOLEAN;
            break;
        case AND: case OR: case NOT:
            if ((t->child[0]->type != KR_INTEGER && t->child[0]->type != KR_BOOLEAN) ||
                (t->child[1]->type != KR_INTEGER && t->child[1]->type != KR_BOOLEAN)) {
                fprintf(stderr,"Type error:%s\n", "Op applied to non-boolean");
                return KR_FAILURE;
            }
            else
                t->type = KR_BOOLEAN;
            break;
        case BL: case NBL:
            if (t->child[0]->type != t->child[1]->type) {
                fprintf(stderr,"Type error:%s\n", "Op applied to non-boolean");
                return KR_FAILURE;
            }
            else
                t->type = KR_BOOLEAN;
            break;
        case MATCH:
            if (t->child[0]->type != KR_STRING) {
                fprintf(stderr,"Type error:%s\n", "Op applied to non-boolean");
                return KR_FAILURE;
            }
            else
                t->type = KR_BOOLEAN;
            break;
        }
        break;
    }
    return KR_SUCCESS;
}

/* procedure transNode transforms
 * a tree node type to the appropriate one 
 */
static int transNode(KRRuletree * t, void *data)
{
    if (t->expkind == OpK) {
        switch(t->attr.op) {
        case PLUS: case SUB: case MUT: case DIV:
            if ((t->type == KR_DOUBLE && t->child[0]->type == KR_INTEGER)) {
                t->child[0]->type = KR_DOUBLE;
                if (t->child[0]->expkind == NumK)
                    t->child[0]->attr.val.d = (double)(t->child[0]->attr.val.i);
            }
            if ((t->type == KR_DOUBLE && t->child[1]->type == KR_INTEGER)) {
                t->child[1]->type = KR_DOUBLE;
                if ( t->child[1]->expkind == NumK)
                    t->child[1]->attr.val.d = (double)(t->child[1]->attr.val.i);
            }
            break;
        }
    }
    return KR_SUCCESS;
}

/* procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
int kr_ruletree_check(KRRuletree * syntaxTree)
{ 
    kr_ruletree_traverse(syntaxTree, NULL, NULL, checkNode);
    kr_ruletree_traverse(syntaxTree, NULL, transNode, NULL);
    return KR_SUCCESS;
}

/* prints a ruletree to the 
 * printer file using indentation to indicate subtrees
 */
void kr_ruletree_print(KRRuletree *root, FILE *printer)
{
    kr_ruletree_traverse(root, printer, 
       kr_ruletree_node_print_pre, kr_ruletree_node_print_post);
}

/* free a ruletree */
void kr_ruletree_free(KRRuletree *root)
{
    kr_ruletree_traverse(root, NULL, kr_ruletree_node_free, NULL);
}