#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <stdio.h>

/* Funzioni per il print dell'Ast */
void print_ast(struct AstNode *n);
void print_node(struct AstNode *n);
void print_list(struct AstNode *l);
void print_decl(struct AstNode *l, char * type);

/* Funzioni per la traduzione in Golang */
void translate(struct AstNode *n);
void translate_tab(int depth);
void translate_ast(struct AstNode *n);
void translate_node(struct AstNode *n);
void translate_list(struct AstNode *l);
void translate_decl(struct AstNode *n, char * type);
void translate_init(struct AstNode *i);
void translate_param_decl(struct AstNode *p);
void translate_init(struct AstNode *i);
void translate_param_list(struct AstNode *l);

/* Funzioni di supporto per convertire di tipi, espressioni, ... */
char * convert_var_type(int t);
char * convert_expr_type(int t);
char * convert_node_type(int t);
char * convert_func_name(char *  name);
char * convert_golang_type (int t);

#endif