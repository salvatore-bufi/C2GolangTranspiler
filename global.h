#ifndef GLOBAL_H
#define GLOBAL_H

#include <string.h>

/* Definizioni delle costanti per i colori nei messaggi di errore */
#define RESET   "\033[0m"
#define YELLOW  "\033[1m\033[33m"
#define RED     "\033[1m\033[31m"
#define BLUE    "\033[1m\033[34m"   
#define BOLD   "\033[1m\033[37m" 

char *filename; 
int error_num;
int main_flag; // package
int fmt_flag; // import "fmt"


int current_scope_lvl; // indica il livello dello scope corrente
struct symlist *current_symtab;


/* funzioni per la gestione degli errori */
void yyerror(const char *s);
void yywarning(char *s);
void yynote(char *s, int lineno, char *line);
char* error_string_format(char *msg, ...);

#endif