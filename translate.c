#include "tree.h"
#include "global.h"
#include <stdlib.h>
#include "translate.h"

int depth = 0; // livello di indentazione
FILE *fp;

/* Funzione per printare l'indentazione */
void print_tab(int d) {
    // d = livello di indentazione
    for(int i = 0; i < d; i++)
        printf("\t");
}

/* Funzione per printare i nodi Ast */
void print_node(struct AstNode *n){
    switch(n->nodetype){
        case VAL_T:
            if(n -> node.val -> val_type == STRING_T) {
                printf("\"");
                printf("%s", n -> node.val -> string_val);
                printf("\"");
            }else{
                printf("%s", n -> node.val -> string_val);
            }
            break;
        case VAR_T:
            if(n -> node.var -> by_reference) {
                printf("&");
            }
            printf("%s", n -> node.var-> name);
            if(n -> node.var -> array_dim) {
                printf("[");
                print_node(n -> node.var -> array_dim);
                printf("]");
            }
            break;
        case DECL_T:
            printf("%s ", convert_var_type(n -> node.decl -> type));
            print_list(n -> node.decl -> var);
            break;
        case EXPR_T:
            if(n -> node.expr -> expr_type == PAR_T) {
                printf("(");
                print_node(n -> node.expr -> r);
                printf(")");
            } else {
                if(n -> node.expr -> l)
                    print_node(n -> node.expr -> l);
                printf("%s", convert_expr_type(n -> node.expr -> expr_type));
                print_node(n -> node.expr -> r);
            }
            break;
        case RETURN_T:
            printf("return ");
            if(n -> node.ret -> expr) {
                print_node(n -> node.ret -> expr);
            }
            break;
        case FCALL_T:
            printf("%s(", n -> node.fcall -> name);
            if(n -> node.fcall -> args) {
                print_list(n-> node.fcall -> args);
            }
            printf(")");
            break;
        case FDEF_T:
            printf("%s ", convert_var_type(n -> node.fdef -> ret_type));
            printf("%s(", n -> node.fdef -> name );
            if (n -> node.fdef -> params){
                print_list(n -> node.fdef -> params);
            }
            printf(") {\n");
            print_ast( n -> node.fdef -> code);
            depth--;
            print_tab(depth);
            printf("}\n\n");
            break;
        case FOR_T:
            printf("for(");
            if(n -> node.forn -> init)
                print_node(n -> node.forn -> init);
            printf("; ");
            if(n -> node.forn -> cond)
                print_node(n -> node.forn -> cond);
            printf("; ");
            if(n -> node.forn -> update)
                print_node(n -> node.forn -> update);
            printf(") {\n");
            print_ast(n -> node.forn -> stmt);
            depth--;
            print_tab(depth);
            printf("}\n");
            break;
        case IF_T:
            printf("if(");
            print_node(n -> node.ifn -> cond);
            printf(") {\n");
            print_ast(n -> node.ifn -> body);
            depth--;
            print_tab(depth);
            printf("}");
            if(n -> node.ifn -> else_body) {
                printf("else{\n");
                depth++;
                print_ast(n -> node.ifn -> else_body);
                depth--;
                print_tab(depth);
                printf("}\n");
            }else{
                printf("\n");
            }
            break;
        case ERROR_NODE_T:
            printf("error node");
    }
}

/* Funzione per convertire il tipo di espressione */
char * convert_expr_type(int t){
    switch(t){
        case ADD_T:
            return "+";
        case SUB_T:
        case NEG_T:
            return "-";
        case DIV_T:
            return "/";
        case MUL_T:
            return "*";
        case NOT_T:
            return "!";
        case AND_T:
            return "&&";
        case OR_T:
            return "||";
        case G_T:
            return ">";
        case GE_T:
            return ">=";
        case L_T:
            return "<";
        case LE_T:
            return "<=";
        case EQ_T:
            return "==";
        case NE_T:
            return "!=";
        case ASS_T:
            return "=";
    }
}

/* Funzione per convertire il tipo di dato */
char * convert_var_type (int t){
    switch(t) {
        case INT_T:
            return "int";
        case FLOAT_T:
            return "float";
        case DOUBLE_T:
            return "double";
        case STRING_T:
            return "string";
        case VOID_T:
            return "void";
        case BOOL_T:
            return "bool";
        default:
            return "unrecognized type";
    }
}

/* Funzione per convertire il tipo di nodo */
// usata per debug
char * convert_node_type(int t) {
    switch(t) {
        case EXPR_T:
            return "expr node";
        case IF_T:
            return "if node";
        case VAL_T:
            return "val node";
        case VAR_T:
            return "var node";
        case DECL_T:
            return "decl node";
        case RETURN_T:
            return "ret node";
        case FCALL_T:
            return "func call node";
        case FDEF_T:
            return "func def node";
        case FOR_T:
            return "for node";
    }
}

/* Funzione per printare l'Ast */
void print_ast(struct AstNode *n) {
    while(n){
        print_tab(depth);
        
        if(n -> nodetype == FDEF_T || n -> nodetype == FOR_T || n -> nodetype == IF_T) {
            depth++;
        }

        print_node(n);

        if(n -> nodetype != FDEF_T && n -> nodetype != FOR_T && n -> nodetype != IF_T){
            printf(";\n");
        }

        n = n->next;
    }
}

/* Funzione per printare liste di nodi */
void print_list(struct AstNode *l) {
    while(l){
        print_node(l);
        l = l->next;
        
        if(l){
            printf(", ");
        }
    }
}

/* Funzione per printare le dichiarazioni multiple come singole dichiarazioni */
void print_decl(struct AstNode *l, char * type){
    /* splitta le dichiarazioni multiple , in quanto in golang non possiamo averle con assegnazione se non sono tutte con
    assegnazione, es int a, b=0, c; */
    while(l){
        print_tab(depth);
        printf("%s ", type);
        print_node(l);
        l = l -> next;
        if(l){
            printf(" ; \n");
        } 
    }
}

/* Funzione principale per la traduzione */
void translate(struct AstNode * n){

    fp = fopen("traduzione.go", "w");

    if(!fp) {
        fprintf(stderr, RED "error:" RESET " %s: ", filename);
        perror("");
        exit(1);
    }
    
    // flag per printare il package
    if(main_flag)
        fprintf(fp, "package main \n\n");
    else
        fprintf(fp, "package test \n\n");

    // flag per printare l'import della libreria fmt
    if(fmt_flag)
        fprintf(fp, "import \"fmt\" \n\n");

    translate_ast(n);

    fclose(fp);
}

/* Funzione per tradurre l'Ast */
void translate_ast(struct AstNode *n){
    while(n){
        if(n -> nodetype != DECL_T){
            translate_tab(depth);
        }
        if(n -> nodetype == FDEF_T || n -> nodetype == FOR_T || n -> nodetype == IF_T) {
            depth++;
        }

        translate_node(n);

        if(n -> nodetype != DECL_T && n -> nodetype != FDEF_T && n -> nodetype != FOR_T && n -> nodetype != IF_T){
            fprintf(fp, "\n");
        }

        n = n->next;
    }
}

/* Funzione per tradurre i singoli nodi dell'Ast */
void translate_node(struct AstNode *n){
    switch(n -> nodetype){
        case VAL_T:
            if(n -> node.val -> val_type == STRING_T) {
                fprintf(fp, "\"");
                fprintf(fp, "%s", n -> node.val -> string_val);
                fprintf(fp, "\"");
            }else{
                fprintf(fp, "%s", n -> node.val -> string_val);
            }
            break;
        case VAR_T:
            if(n -> node.var -> by_reference) {
                fprintf(fp, "&");
            }
            fprintf(fp,"%s", n -> node.var-> name);
            if(n -> node.var -> array_dim) {
                fprintf(fp, "[");
                translate_node(n -> node.var -> array_dim);
                fprintf(fp, "]");
            }
            break;
        case DECL_T:
            translate_decl(n -> node.decl -> var, convert_golang_type(n -> node.decl -> type));
            break;
        case EXPR_T:
            if(n -> node.expr -> expr_type == PAR_T) {
                fprintf(fp,"(");
                translate_node(n -> node.expr -> r);
                fprintf(fp, ")");
            } else {
                if(n -> node.expr -> l)
                    translate_node(n -> node.expr -> l);
                fprintf(fp, "%s", convert_expr_type(n -> node.expr -> expr_type));
                translate_node(n -> node.expr -> r);
            }
            break;
        case RETURN_T:
            if(n -> node.ret -> expr) {
                fprintf(fp,"return ");
                translate_node(n -> node.ret -> expr);
            }
            break;
        case FCALL_T:
            fprintf(fp,"%s(", convert_func_name(n -> node.fcall -> name));
            if(n -> node.fcall -> args) {
                translate_list(n-> node.fcall -> args);
            }
            fprintf(fp, ")");
            break;
        case FDEF_T:
            fprintf(fp,"func ");
            fprintf(fp,"%s(", n -> node.fdef -> name );
            if (n -> node.fdef -> params){
                translate_param_list(n -> node.fdef -> params);
            }
            fprintf(fp, ") ");
            fprintf(fp, "%s ", convert_golang_type(n -> node.fdef -> ret_type));
            fprintf(fp," {\n");
            translate_ast( n -> node.fdef -> code);
            depth--;
            translate_tab(depth);
            fprintf(fp,"}\n\n");
            break;
        case FOR_T:
            fprintf(fp,"for ");
            if(n -> node.forn -> init)
                translate_init(n -> node.forn -> init);
            fprintf(fp,"; ");
            if(n -> node.forn -> cond)
                translate_node(n -> node.forn -> cond);
            fprintf(fp,"; ");
            if(n -> node.forn -> update)
                translate_node(n -> node.forn -> update);
            fprintf(fp, " {\n");
            translate_ast(n -> node.forn -> stmt);
            depth--;
            translate_tab(depth);
            fprintf(fp, "}\n");
            break;
        case IF_T:
            fprintf(fp, "if ");
            translate_node(n -> node.ifn -> cond);
            fprintf(fp," {\n");
            translate_ast(n -> node.ifn -> body);
            depth--;
            translate_tab(depth);
            fprintf(fp,"}");
            if(n -> node.ifn -> else_body) {
                fprintf(fp,"else{\n");
                depth++;
                translate_ast(n -> node.ifn -> else_body);
                depth--;
                translate_tab(depth);
                fprintf(fp,"}\n");
            }else{
                fprintf(fp,"\n");
            }
            break;
    }
}

/* Funzione per tradurre liste di nodi */
void translate_list(struct AstNode *l) {
    while(l){
        translate_node(l);
        l = l->next;
        
        if(l){
            fprintf(fp,", ");
        }
    }
}

/* Funzione per tradurre liste di parametri */
void translate_param_list(struct AstNode *l){
    while(l){
        translate_param_decl(l);
        l = l->next;
        
        if(l){
            fprintf(fp,", ");
        }
    }
}

/* Funzione per tradurre dichiarazioni di parametri */
void translate_param_decl(struct AstNode *p){
    // in golang abbiamo un ordine differente per le dichiarazioni di parametri
    // esempio: func f(a int, b[] int)
    fprintf(fp,"%s", p -> node.decl -> var -> node.var-> name);
    if(p -> node.decl -> var -> node.var -> array_dim)
        fprintf(fp,"[]");
    fprintf(fp," %s", convert_golang_type(p -> node.decl -> type));
}

/* Funzione per tradurre l'init del for */
void translate_init(struct AstNode *i){
    // in golang è possibile la dichiarazione nell'init solo con la sintassi
    // for(i := 0; ...; ...)
    if(i -> nodetype == DECL_T && i -> node.decl -> var -> nodetype == EXPR_T){
        translate_node(i -> node.decl -> var -> node.expr -> l);
        fprintf(fp," := ");
        translate_node(i -> node.decl -> var -> node.expr -> r);
    }else{
        translate_node(i);
    }
}

/* Funzione per tradurre i nodi dichiarazione */
void translate_decl(struct AstNode *n, char *type){
    // divide le dichiarazioni multiple in più dichiarazioni singole
    while(n){
        translate_tab(depth);
        fprintf(fp,"var ");
        if(n -> nodetype == VAR_T){
            fprintf(fp,"%s", n -> node.var-> name);
            if(n -> node.var -> array_dim){
                fprintf(fp," = make([]%s, ", type);
                translate_node(n-> node.var ->array_dim);
                fprintf(fp, ",");
                translate_node(n-> node.var ->array_dim);
                fprintf(fp, ")");
            } else {
                fprintf(fp, " %s", type);
            }
        }else if(n -> nodetype == EXPR_T){
            translate_node(n -> node.expr -> l);
            fprintf(fp, " %s %s ", type, convert_expr_type(n -> node.expr -> expr_type));
            translate_node(n -> node.expr -> r);
        }
        fprintf(fp, "\n");
        n = n -> next;
    }
}

/* Funzione per inserire l'indentazione nella traduzione */
void translate_tab(int d) {
    for(int i = 0; i < d; i++)
        fprintf(fp, "\t");
}

/* Funzione per convertire gli identificatori delle funzioni per I/O */
char * convert_func_name(char *name){
    if (!strcmp("printf", name))
            return "fmt.Printf";
    if (!strcmp("scanf", name))
            return "fmt.Scanf";
    return name;
}

/* Funzione per convertire i tipi di dato nei tipi accettati da golang */
char * convert_golang_type (int t){
    switch(t) {
        case INT_T:
            return "int";
        case FLOAT_T:
            return "float32";
        case DOUBLE_T:
            return "float64";
        case STRING_T:
            return "string";
        case VOID_T:
            return "";
        case BOOL_T:
            return "bool";
        default:
            return "unrecognized type";
    }
}