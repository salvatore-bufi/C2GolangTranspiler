#ifndef TREE_H
#define TREE_H

// Tipo di dato
enum TYPE {
    BOOL_T,
    INT_T,
    FLOAT_T,
    DOUBLE_T,
    VOID_T,
    STRING_T,  
    ERROR_T
};

// Tipo di nodo
enum NODE_TYPE {
    EXPR_T,
    IF_T,
    VAL_T,
    VAR_T,
    DECL_T,
    RETURN_T,
    FCALL_T,
    FDEF_T,
    FOR_T,
    ERROR_NODE_T
};

// Tipo di espressione
enum EXPRESSION_TYPE {
    ASS_T,
    ADD_T,
    SUB_T,
    DIV_T,
    MUL_T,
    NOT_T,
    AND_T,
    OR_T,
    G_T, 
    GE_T,
    L_T,
    LE_T,
    EQ_T,
    NE_T, 
    NEG_T,
    PAR_T
};

// Struttura del nodo espressione e assignment
struct expression {
    enum EXPRESSION_TYPE expr_type;
    struct AstNode *l;
    struct AstNode *r;
    /* dato che hanno stessa struttura li abbiamo uniti
    struct assignment {
        struct AstNode *var;
        struct AstNode *r;
    }; */
};

// Struttura del nodo if 
struct ifNode {
    struct AstNode *cond;
    struct AstNode *body;
    struct AstNode *else_body;
};

// Struttura del nodo for
struct forNode {
    struct AstNode *init;
    struct AstNode *cond;
    struct AstNode *update;
    struct AstNode *stmt;
};

// Struttura del nodo valore
struct value {
    enum TYPE val_type;
    char *string_val;
};

// Struttura del nodo variabile
struct variable {
    char *name;
    int by_reference; // indica se è stato usato l'operatore di referenziazione &
    struct AstNode *array_dim; // indica sia la dimensione che l'indice di un array
};

// Struttura del nodo dichiarazione
struct declaration {
    enum TYPE type;
    struct AstNode *var;
};

// Struttura del nodo return
struct returnNode {
    struct AstNode *expr;
};

// Struttura del nodo chiamata a funzione
struct funcCall {
    char *name;
    struct AstNode *args;
};

// Struttura del nodo definizione di funzione
struct funcDef {
    enum TYPE ret_type;
    char *name;
    struct AstNode *params;
    struct AstNode *code;
};


// Struttura del nodo generico Ast
struct AstNode {
    enum NODE_TYPE nodetype;

    union node{
        struct variable *var;
        struct value *val;
        struct expression *expr;
        struct ifNode *ifn;
        struct forNode *forn;
        struct declaration *decl;
        struct returnNode *ret;
        struct funcCall *fcall;
        struct funcDef *fdef;
    }node;

    struct AstNode *next;
};

/* Funzioni per creare i nodi */
struct AstNode* new_value(enum NODE_TYPE nodetype, enum TYPE val_type, char *string_val);
struct AstNode* new_return(enum NODE_TYPE nodetype, struct AstNode *expr);
struct AstNode* new_variable(enum NODE_TYPE nodetype, char* name, struct AstNode *array_dim);
struct AstNode* new_expression(enum NODE_TYPE nodetype, enum EXPRESSION_TYPE expr_type, struct AstNode* l, struct AstNode* r);
struct AstNode* new_declaration(enum NODE_TYPE nodetype, enum TYPE type, struct AstNode *var);
struct AstNode* new_func_call(enum NODE_TYPE nodetype, char *name, struct AstNode* args);
struct AstNode* new_func_def(enum NODE_TYPE nodetype, enum TYPE ret_type, char *name, struct AstNode* params, struct AstNode* code);
struct AstNode* new_for(enum NODE_TYPE nodetype, struct AstNode *init, struct AstNode* cond, struct AstNode* update, struct AstNode* stmt);
struct AstNode* new_if(enum NODE_TYPE nodetype, struct AstNode *cond, struct AstNode *body, struct AstNode *else_body);
struct AstNode* new_error(enum NODE_TYPE nodetype);

/* Funzione per modificare gli attributi dei nodi */
void by_reference(struct AstNode *node);

/* Funzioni per linkare due nodi */
struct AstNode* link_AstNode(struct AstNode *node, struct AstNode *next);
struct AstNode* append_AstNode(struct AstNode *node, struct AstNode *next);

#endif