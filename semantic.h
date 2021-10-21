#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "tree.h"

/* Tipo di funzione built-in */
enum FUNC_TYPE {
    PRINTF_T,
    SCANF_T
};

/* Tipo complesso usato per eval_expr_type */
struct complex_type{
    enum TYPE type;
    enum KIND{
        V,
        N
    }kind;
};

/* Funzioni di controllo per le funzioni */
void check_fcall(char * name, struct AstNode *args);
void check_return(struct AstNode *expr);
void check_func_return(enum TYPE type, struct AstNode *code);
int check_if_return(struct AstNode *tmp);
void check_main(struct AstNode *fdecl);

/* Funzioni di controllo per gli array */
void check_array(struct AstNode *dim);
int check_array_dim(struct AstNode *expr);
int eval_array_dim(struct AstNode *expr);

/* Funzione per impostare l'utilizzo delle variabili nello scope */
void check_var_reference(struct AstNode *var);

/* Funzioni di controllo per format string */
char* replace_format(char *string, char *old, char *new);
struct AstNode * extract_format_specifier(struct AstNode *format_string, enum FUNC_TYPE f_type);
void check_format_string(struct AstNode* format_string, struct AstNode* args, enum FUNC_TYPE f_type);

/* Funzioni di controllo per le espressioni */
struct complex_type eval_expr_type (struct AstNode *expr);
struct complex_type eval_comparison_op_type(struct complex_type l, struct complex_type r, enum EXPRESSION_TYPE expr_type);
struct complex_type eval_arithmetic_op_type(struct complex_type l, struct complex_type r);
struct complex_type eval_ass_op(struct complex_type l, struct complex_type r, struct AstNode *r_node);
void check_cond(enum TYPE type);
struct AstNode * check_expr_statement(struct AstNode *expr);
float eval_constant_expr(struct AstNode *expr);
void check_division(struct AstNode *expr);
float check_const_truncated(struct AstNode * expr );
#endif