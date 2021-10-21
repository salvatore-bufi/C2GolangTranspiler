#include "symtab.h"
#include "tree.h"
#include "translate.h"
#include "global.h"
#include <stdio.h>

/* Crea una nuova symbol table */
struct symlist* create_symtab(int scope, struct symlist *next) {
    /*  scope = numero identificativo dello scope
        next = puntatore alla tabella precedente (a scope più esterno)
    */
    struct symbol *symtab = NULL; // creo una nuova tabella vuota
    struct symlist *syml = malloc(sizeof(struct symlist));

    syml -> scope = scope;
    syml -> symtab = symtab;
    syml -> next = next;

    return syml;
}

/* Elimina una symbol table */
struct symlist* delete_symtab(struct symlist *syml) {
    /*  syml = tabella da eliminare */
    struct symbol *s, *tmp;

    HASH_ITER(hh, syml -> symtab, s, tmp) {
        HASH_DEL(syml -> symtab, s);

        free(s -> line);
        free(s);
    }
    
    struct symlist *next;
    next = syml -> next;
    free(syml);
    return next;
}

/* Printa la Symbol Table */
void print_symtab(struct symlist *syml) {
    struct symbol *s, *tmp;

    printf("\nSYMBOL TABLE \t scope: %d \n", syml -> scope);
    printf("---------------------------\n");
    HASH_ITER(hh, syml -> symtab, s, tmp) {
        printf("simbolo: %s \t tipo: %s \n", s->name, convert_var_type(s-> type));
    }
    printf("---------------------------\n\n");
}


/*  Cerca simboli all'interno di tutti gli scope 
    attualmente aperti partendo dallo scope corrente 
*/
struct symbol* find_symtab(struct symlist *syml, char *name) {
    /*  syml = tabella dello scope corrente 
        name = nome del simbolo da cercare 
    */
    struct symlist *tmp = syml;
    struct symbol *s;
    
    while(tmp) {
        s = find_sym(tmp, name);
        
        if(s) 
            return s;
        tmp = tmp -> next;
    }
    
    return NULL;
}

/* Inserisce un simbolo all'interno dello scope corrente */
void insert_sym(struct symlist *syml, char *name, enum TYPE type, enum sym_type sym_type, struct AstNode *pl, int lineno, char *line) {
    /*  syml = tabella dello scope corrente
        name = identificatore del simbolo da inserire
        type = tipo di dato
        sym_type = tipo del simbolo
        pl = puntatore alla lista dei parametri (se il simbolo è una funzione), usato per il check sulle f_call
        lineno = numero di riga della dichiarazione del simbolo
        line = riga della dichiarazione del simbolo
    */
    struct symbol *s;

    s = find_sym(syml, name);

    if(s) {
        yyerror(error_string_format(BOLD "%s" RESET " redeclared in this scope", name));
        yynote(error_string_format("previous declaration of " BOLD "%s" RESET " was here", name), s -> lineno, s -> line);
    } else {
        s = malloc(sizeof(struct symbol));
        s -> name = name;
        s -> type = type;
        s -> sym_type = sym_type;
        s -> pl = pl;
        s -> lineno = lineno;
        s -> line = strdup(line);
        s -> used_flag = 0;
        s -> array_flag = 0;

        HASH_ADD_STR(syml -> symtab, name, s);
    }
}

/* Ricerca di un simbolo nello scope corrente */
struct symbol* find_sym(struct symlist* syml, char *name) {
    /*  syml = symbol table corrente
        name = identificatore da cercare
    */
    struct symbol *s;

    HASH_FIND_STR(syml -> symtab, name, s);

    if(s)
        return s;
    
    return NULL;
}


/* Rimuove un simbolo dalla symbol table passata */
void remove_sym(struct symlist* syml, struct symbol *s) {
    /*  syml = symbol table
        s = simbolo da rimuovere
    */
    HASH_DEL(syml -> symtab, s);

    free(s -> line);
    free(s);
}

/* Controlla se tutte le variabili presenti nella Symbol Table sono state utilizzate */
void check_usage(struct symlist *syml){
    /*  syml = symbol table a controllare 
    */
    struct symbol *s;
    
    // hh.next = per iterare sugli hash, equivale a fare il next del simbolo 
    for (s = syml -> symtab; s != NULL; s = s -> hh.next){
        if (s -> sym_type == VARIABLE && s -> used_flag == 0){
            yyerror(error_string_format("variable " BOLD "%s" RESET " declared but not used", s -> name));
            yynote(error_string_format("declaration of " BOLD "%s" RESET " was here", s -> name), s -> lineno, s -> line);
        }
    }
}