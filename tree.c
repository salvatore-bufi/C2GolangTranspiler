#include "tree.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>

/* Crea un nodo valore */
struct AstNode* new_value(enum NODE_TYPE nodetype, enum TYPE val_type, char *string_val) {
    /*  nodetype = VAL_T
        val_type = tipo di dato
        string_val = valore 
    */
    struct value *val = malloc(sizeof(struct value));
    struct AstNode *node = malloc(sizeof(struct AstNode));

    val -> val_type = val_type;
    val -> string_val = string_val;

    node -> nodetype = nodetype;
    node -> node.val = val;
    node -> next = NULL;

    return node;
}


/* Crea un nodo variabile */
struct AstNode* new_variable(enum NODE_TYPE nodetype, char* name, struct AstNode *array_dim) {
    /*  nodetype = VAR_T
        name = nome della variabile
        array_dim = dimensione dell'array
    */
    struct variable *var = malloc(sizeof(struct variable));
    struct AstNode *node = malloc(sizeof(struct AstNode));

    var -> name = name;
    var -> array_dim = array_dim;
    var -> by_reference = 0;

    node -> nodetype = nodetype;
    node -> node.var = var;
    node -> next = NULL;

    return node;
}

/* Modifica il valore del campo reference di un nodo di tipo VAR_T */
void by_reference(struct AstNode * node){
    node -> node.var -> by_reference = 1;
}

/* Crea un nodo dichiarazione */
struct AstNode* new_declaration(enum NODE_TYPE nodetype, enum TYPE type, struct AstNode *var) {
    /*  nodetype = DECL_T
        type = tipo della dichiarazione (int, float, ...)
        var = puntatore alla lista delle variabili dichiarate 
        (può essere anche nodo EXPR_T in caso di dichiarazioni con assegnazione)
    */
    struct declaration *decl = malloc(sizeof(struct declaration));
    struct AstNode *node = malloc(sizeof(struct AstNode));
    
    decl -> type = type;
    decl -> var = var;

    node -> nodetype = nodetype;
    node -> node.decl = decl;
    node -> next = NULL;

    // controllo sul tipo delle variabili (iterazione per dichiarazioni multiple)
    if (type == VOID_T){
        for(struct AstNode *tmp = var; tmp; tmp = tmp -> next){
            yyerror(error_string_format("variable " BOLD "%s" RESET " declared void", tmp -> node.var -> name));
        }
    }
    
    return node;
}


/* Crea un nodo espressione */
struct AstNode* new_expression(enum NODE_TYPE nodetype, enum EXPRESSION_TYPE expr_type, struct AstNode* l, struct AstNode* r){
    /*  nodetype = EXPR_T
        expr_type = tipo di espressione
        l = operatore destro
        r = operatore sinistro
    */
    struct expression *expr = malloc(sizeof(struct expression));
    struct AstNode *node = malloc(sizeof(struct AstNode));

    expr -> expr_type = expr_type;
    expr -> l = l;
    expr -> r = r;

    node -> nodetype = nodetype;
    node -> node.expr = expr;
    node -> next = NULL;

    return node;
}

/* Crea un nuovo nodo return */
struct AstNode* new_return(enum NODE_TYPE nodetype, struct AstNode *expr){
    /*  nodetype = RETURN_T
        expr = espressione o valore ritornato
    */
    struct returnNode *rnode = malloc(sizeof(struct returnNode));
    struct AstNode *node = malloc(sizeof(struct AstNode));
    
    rnode -> expr = expr;
    
    node -> nodetype = nodetype;
    node -> node.ret = rnode;
    node -> next = NULL;

    return node;
}


/* Crea un nodo chiamata a funzione */
struct AstNode* new_func_call(enum NODE_TYPE nodetype, char *name, struct AstNode* args) {
    /*   nodetype = FCALL_T
         name = nome della funzione
         args = puntatore alla lista di argomenti della chiamata a funzione
    */
    
    struct funcCall *fcall = malloc(sizeof(struct funcCall));
    struct AstNode *node = malloc(sizeof(struct AstNode));

    fcall -> name = name;
    fcall -> args = args;

    node -> nodetype = nodetype;
    node -> node.fcall = fcall;
    node -> next = NULL;

    return node;
}

/* Crea un nodo definizione di funzione */
struct AstNode* new_func_def(enum NODE_TYPE nodetype, enum TYPE ret_type, char *name, struct AstNode* params, struct AstNode* code){
    /*  nodetype = FDEF_T
        ret_type = tipo di ritorno della funzione
        name = identificatore della funzione
        params = puntatore alla lista dei parametri della funzione
        code = puntatore alla lista di istruzioni contenute nella definizione della funzione
    */
    struct funcDef *fdef = malloc(sizeof(struct funcDef));
    struct AstNode *node = malloc(sizeof(struct AstNode));

    fdef -> ret_type = ret_type;
    fdef -> name = name;
    fdef -> params = params;
    fdef -> code = code;

    node -> nodetype = nodetype;
    node -> node.fdef = fdef;
    node -> next = NULL;

    return node;
}

/* Crea un nodo for */
struct AstNode* new_for(enum NODE_TYPE nodetype, struct AstNode *init, struct AstNode* cond, struct AstNode* update, struct AstNode* stmt){
    /*  nodetype = FOR_T
        init = puntatore al nodo ast che contiene l'init del for
        cond = puntatore al nodo ast che contiene la condizione del for
        update = puntatore al nodo ast che contiene l'update del for
        stmt = puntatore al nodo ast che contiene il corpo del for
    */
    struct forNode *forn = malloc(sizeof(struct forNode));
    struct AstNode *node = malloc(sizeof(struct AstNode));

    forn -> init = init;
    forn -> cond = cond;
    forn -> update = update;
    forn -> stmt = stmt;

    node -> nodetype = nodetype;
    node -> node.forn = forn;
    node -> next = NULL;

    return node;
}

/* Crea un nodo if */
struct AstNode* new_if(enum NODE_TYPE nodetype, struct AstNode *cond, struct AstNode *body, struct AstNode *else_body) {
    /*  nodetype = IF_T
        cond = puntatore al nodo Ast che contiene la condizione del if
        body = puntatore al nodo Ast che contiene il body dell'if
        else_body = puntatore al nodo Ast che contiene il body dell'else (se presente) 
    */
    struct ifNode *ifn = malloc(sizeof(struct ifNode));
    struct AstNode *node = malloc(sizeof(struct AstNode));

    ifn -> cond = cond;
    ifn -> body = body;
    ifn -> else_body = else_body;

    node -> nodetype = nodetype;
    node -> node.ifn = ifn;
    node -> next = NULL;

    return node;
}

/* Crea un nodo errore */
struct AstNode* new_error(enum NODE_TYPE nodetype){
    /* nodetype = ERROR_NODE_T */
    struct AstNode *node =  malloc(sizeof(struct AstNode));
    
    node -> nodetype = nodetype;
    node -> next = NULL;
    return node;
}

/* Funzione usata per creare una lista di nodi Ast partendo dall'ultimo 
  es A, B, C  ==>   A ----2----> B ----1----> C restituisce A*/
struct AstNode* link_AstNode(struct AstNode *node, struct AstNode *next){
    node -> next = next;
    return node;
}

/* Funzione usata per creare una lista di nodi Ast partendo dal primo 
   es A, B, C  ==>   A ----1----> B ----2----> C restituisce C */
struct AstNode* append_AstNode(struct AstNode *node, struct AstNode *next){
    node -> next = next;
    return next;
}