#ifndef SYMTAB_H
#define SYMTAB_H

#include "uthash.h"
#include "tree.h"

// Tipi di simboli
enum sym_type {
    VARIABLE,
    FUNCTION,
    PARAMETER,
    F_RETURN
};


// struttura del simbolo
struct symbol {
    char *name; // chiave della hash table
    enum TYPE type;
    enum sym_type sym_type;
    struct AstNode *pl;
    int used_flag;
    int array_flag;
    int lineno;
    char *line;

    UT_hash_handle hh; //per rendere la struttura "hashable"
};

// lista di tabelle
struct symlist {
    int scope;
    struct symbol *symtab; // tabella dello scope corrente 
    struct symlist *next; // tabella successiva
};

// gestione delle tabelle 
struct symlist* create_symtab(int scope, struct symlist *next);
struct symlist* delete_symtab(struct symlist *syml);
struct symbol* find_symtab(struct symlist *syml, char *name);
void print_symtab(struct symlist *syml);
void check_usage(struct symlist *syml);

// gestione dei singoli simboli
void insert_sym(struct symlist* syml, char *name, enum TYPE type, enum sym_type sym_type, struct AstNode *pl, int lineno, char *line);
struct symbol* find_sym(struct  symlist* syml, char *name);
void remove_sym(struct  symlist* syml, struct symbol *s);

#endif