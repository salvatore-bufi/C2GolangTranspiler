%{
#include <stdio.h>
#include <stdlib.h>
#include "symtab.h"
#include "tree.h"
#include "global.h"
#include "semantic.h"
#include "translate.h"

extern int yylex();
extern FILE *yyin;
extern int yylineno;
extern char *line;

struct AstNode *root;
struct AstNode *param_list = NULL; // puntatore alla lista dei parametri di una funzione, da inserire nello scope
enum TYPE ret_type = -1; // tipo di ritorno della funzione, da inserire nello scope

void scope_enter();
void scope_exit();
void fill_symtab (struct symlist * syml, struct AstNode *node, enum TYPE type, enum sym_type sym_type);

int print_symtab_flag = 0;
int print_ast_flag = 0;
void print_usage();

%}

%define parse.error verbose

%union {
    char* s;
    struct AstNode *ast;
    int t;
}

%token INT FLOAT DOUBLE VOID
%token <s> INT_NUM FLOAT_NUM STRING
%token IF ELSE FOR RETURN 
%token <s> ID SCANF PRINTF
%left AND OR NOT
%left '>' '<' GE LE EQ NE 
%left '+' '-'
%left '*' '/'
%right MINUS

%expect 1 /* conflitto shift/reduce (else) , bison lo risolve scegliendo shift */

%type <ast> number var var_declaration global_declaration_list global_declaration  expr assignment_statement assignment
%type <ast> return_statement expr_statement func_call args format_string printf_statement  if_cond
%type <ast> declarator_list declarator
%type <ast> func_declaration param_list param compound_statement statement_list statement
%type <ast> scanf_statement scanf_var_list iteration_statement init cond update selection_statement
%type <ast> embedded_statement single_statement
%type <t> type

%%

program
    : { scope_enter(); } global_declaration_list                   { root = $2; scope_exit(); }
    ;

global_declaration_list
    : global_declaration 
    | global_declaration global_declaration_list                    { $$ = link_AstNode($1, $2); }
    ;

global_declaration
    : var_declaration
    | func_declaration                                              { check_main($1); }
    ;

var_declaration
	: type declarator_list ';'                                      { $$ = new_declaration(DECL_T, $1, $2); fill_symtab(current_symtab, $$, -1, VARIABLE); }  
    | ID declarator_list ';'                                        { $$ = new_error(ERROR_NODE_T); yyerror(error_string_format("Unknown type name: " BOLD "%s" RESET, $1 )); }
    ;

declarator_list
	: declarator
	| declarator ',' declarator_list                                { $$ = link_AstNode($1, $3); }
	;

declarator
	: var  
	| assignment
	;

type
    : INT                                                           { $$ = INT_T; }
    | FLOAT                                                         { $$ = FLOAT_T; }
    | DOUBLE                                                        { $$ = DOUBLE_T; }
    | VOID                                                          { $$ = VOID_T; } 
    ;

var
    : ID                                                            { $$ = new_variable(VAR_T, $1, NULL); }
    | ID '[' expr ']'                                               { $$ = new_variable(VAR_T, $1, $3); check_array($3); }
    ;

func_declaration
    : type ID '(' param_list ')'                                    { param_list = $4; ret_type = $1; insert_sym(current_symtab, $2, $1, FUNCTION, $4, yylineno, line); } 
            compound_statement                                      { $$ = new_func_def(FDEF_T, $1, $2, $4, $7); check_func_return($1, $7); }
    | type ID '(' ')'                                               { ret_type = $1; insert_sym(current_symtab, $2, $1, FUNCTION, NULL, yylineno, line); } 
            compound_statement                                      { $$ = new_func_def(FDEF_T, $1, $2, NULL, $6); check_func_return($1, $6); }
    | type ID '(' VOID ')'                                          { ret_type = $1; insert_sym(current_symtab, $2, $1, FUNCTION, NULL, yylineno, line); } 
            compound_statement                                      { $$ = new_func_def(FDEF_T, $1, $2, NULL, $7); check_func_return($1, $7); }
    ;

param_list
    : param  
    | param ',' param_list                                          { $$ = link_AstNode($1, $3); }
    ;

param
    : type ID                                                       { $$ = new_declaration(DECL_T, $1, new_variable(VAR_T, $2, NULL)); }
    | type ID '[' ']'                                               { $$ = new_declaration(DECL_T, $1, new_variable(VAR_T, $2, new_value(VAL_T, INT_T, "0"))); }
    ;

compound_statement
    : scope_enter statement_list scope_exit                         { $$ = $2; }
    ;

scope_enter
    : '{'                                                           { scope_enter(); }
    ;

scope_exit
    : '}'                                                           { scope_exit(); }
    ;

statement_list
    : statement   
    | statement statement_list                                      { $$ = link_AstNode($1, $2); }
    ;

statement
    : var_declaration
    | compound_statement
    | expr_statement
    | selection_statement
    | iteration_statement
    | return_statement
    | printf_statement                                              { fmt_flag = 1; }
    | scanf_statement                                               { fmt_flag = 1; }
    | assignment_statement
    ;


embedded_statement
    : compound_statement
    | single_statement
    ;

single_statement
    : var_declaration                                               { yyerror("embedded statement cannot be a declaration"); }
    | expr_statement
    | selection_statement
    | iteration_statement
    | return_statement
    | printf_statement                                              { fmt_flag = 1; }
    | scanf_statement                                               { fmt_flag = 1; }
    | assignment_statement
    ;

expr_statement
    : expr ';'                                                      { eval_expr_type($1); $$ = check_expr_statement($1); }
    ;

selection_statement
    : IF '(' if_cond ')' embedded_statement                         { $$ = new_if(IF_T, $3, $5, NULL); }
    | IF '(' if_cond ')' embedded_statement ELSE embedded_statement { $$ = new_if(IF_T, $3, $5, $7); }
    ;

if_cond
    : expr                                                          { check_cond(eval_expr_type($1).type); $$ = $1; }                                                           
    ;

iteration_statement
    : FOR '(' init ';' cond ';' update ')' embedded_statement       { $$ = new_for(FOR_T, $3, $5, $7, $9); scope_exit(); }
    ;

init
    : assignment                                                    { scope_enter(); eval_expr_type($1); $$ = $1;}
    | type assignment                                               { scope_enter(); $$ = new_declaration(DECL_T, $1, $2); fill_symtab(current_symtab, $$, -1, VARIABLE); } 
    | /* empty */                                                   { scope_enter(); $$ = NULL; }
    ;

cond
    : expr                                                          { check_cond(eval_expr_type($1).type); $$ = $1; }
    | /* empty */                                                   { $$ = NULL; }
    ;

update
    : assignment                                                    { eval_expr_type($1); $$ = $1; }
    | expr                                                          { eval_expr_type($1); $$ = check_expr_statement($1); }
    | /* empty */                                                   { $$ = NULL; } 
    ;
    
return_statement
    : RETURN ';'                                                    { $$ = new_return(RETURN_T, NULL); check_return(NULL); }
    | RETURN expr ';'                                               { $$ = new_return(RETURN_T, $2); check_return($2); }
    ;

printf_statement
    : PRINTF '(' format_string ')' ';'                              { $$ = new_func_call(FCALL_T, $1, $3); check_format_string($3, NULL, PRINTF_T); }
    | PRINTF '(' format_string ',' args ')' ';'                     { $$ = new_func_call(FCALL_T, $1, link_AstNode($3,$5)); check_format_string($3, $5, PRINTF_T); }
    | PRINTF '(' ')' ';'                                            { $$ = new_error(ERROR_NODE_T); yyerror("too few arguments to function" BOLD " printf" RESET); }
    ;


scanf_statement
    : SCANF '(' format_string ',' scanf_var_list ')' ';'            { check_format_string($3, $5, SCANF_T); $$ = new_func_call(FCALL_T, $1, link_AstNode($3, $5)); }
    | SCANF '(' format_string ')' ';'                               { check_format_string($3, NULL, SCANF_T); $$ = new_func_call(FCALL_T, $1, $3); }
    | SCANF '(' ')' ';'                                             { $$ = new_error(ERROR_NODE_T); yyerror("too few arguments to function" BOLD " scanf" RESET); }
    ;

scanf_var_list
    : '&' var                                                       { check_var_reference($2); by_reference($2); $$ = $2; }
    | var                                                           { check_var_reference($1); $$ = $1; }
    | '&' var ',' scanf_var_list                                    { check_var_reference($2); by_reference($2); $$ = link_AstNode($2, $4); }
    | var ','   scanf_var_list                                      { check_var_reference($1); link_AstNode($1, $3); }
    ;

format_string
    : STRING                                                        { $$ = new_value(VAL_T, STRING_T, $1); }
    ;

assignment_statement
    : assignment ';'                                                { $$ = $1; eval_expr_type($1); }
    ;

assignment
    : var '=' expr                                                  { $$ = new_expression(EXPR_T, ASS_T, $1, $3); }
    ;

expr
    : var                                                           { $$ = $1; check_var_reference($1); }
    | number
    | func_call
    | expr '+' expr                                                 { $$ = new_expression(EXPR_T, ADD_T, $1, $3); }
    | expr '-' expr                                                 { $$ = new_expression(EXPR_T, SUB_T, $1, $3); }
    | expr '*' expr                                                 { $$ = new_expression(EXPR_T, MUL_T, $1, $3); }
    | expr '/' expr                                                 { $$ = new_expression(EXPR_T, DIV_T, $1, $3); check_division($3); }
    | NOT expr                                                      { $$ = new_expression(EXPR_T, NOT_T, NULL, $2); }
    | expr AND expr                                                 { $$ = new_expression(EXPR_T, AND_T, $1, $3); }
    | expr OR expr                                                  { $$ = new_expression(EXPR_T, OR_T, $1, $3); }
    | expr '>' expr                                                 { $$ = new_expression(EXPR_T, G_T, $1, $3); }
    | expr '<' expr                                                 { $$ = new_expression(EXPR_T, L_T, $1, $3); }
    | expr GE expr                                                  { $$ = new_expression(EXPR_T, GE_T, $1, $3); }
    | expr LE expr                                                  { $$ = new_expression(EXPR_T, LE_T, $1, $3); }
    | expr EQ expr                                                  { $$ = new_expression(EXPR_T, EQ_T, $1, $3); }
    | expr NE expr                                                  { $$ = new_expression(EXPR_T, NE_T, $1, $3); }
    | '(' expr ')'                                                  { $$ = new_expression(EXPR_T, PAR_T, NULL, $2); }
    | '-' expr %prec MINUS                                          { $$ = new_expression(EXPR_T, NEG_T, NULL, $2); }
    ;

number
    : INT_NUM                                                       { $$ = new_value(VAL_T, INT_T, $1); }
    | FLOAT_NUM                                                     { $$ = new_value(VAL_T, FLOAT_T, $1); }
    ;

func_call
    : ID '(' args ')'                                               { $$ = new_func_call(FCALL_T, $1, $3); check_fcall($1, $3); }
    | ID '(' ')'                                                    { $$ = new_func_call(FCALL_T, $1, NULL); check_fcall($1, NULL); }
    ;

args
    : expr
    | expr ',' args                                                 { $$ = link_AstNode($1, $3); }
    ;

%%


int main(int argc, char **argv) {
    int file_count = 0;

    if(argc < 2) {
        fprintf(stderr, RED "fatal error:" RESET " no input file\n");
        exit(1);
    }else{
        for(int i = 1; i < argc; i++){
            if(strcmp(argv[i], "-s") == 0)
                print_symtab_flag = 1;
            else if(strcmp(argv[i], "-t") == 0)
                print_ast_flag = 1;
            else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
                print_usage();
                exit(0);
            }else {
                if(argv[i][0] == '-'){
                    fprintf(stderr, RED "error:" RESET " unrecognized command line option " BOLD "%s \n" RESET, argv[i]);
                    print_usage();
                    exit(1);
                }else{
                    if(file_count == 0){
                        filename = argv[i];
                        file_count++;
                    }else{
                        fprintf(stderr, RED "error:" RESET " only one input file accepted \n");
                        exit(1);
                    }
                }
            }
        }
    }

    if(file_count == 0){
        fprintf(stderr, RED "fatal error:" RESET " no input file\n");
        exit(1);
    }else{
        yyin = fopen(filename, "r");
    }

    if(!yyin) {
        fprintf(stderr, RED "error:" RESET " %s: ", filename);
        perror("");
        fprintf(stderr, RED "fatal error:" RESET" no input file\n");
        exit(1);
    }

    // inizializzazione variabili globali
    error_num = 0;
    current_scope_lvl = 0;
    current_symtab = NULL;

    if(yyparse() == 0) {
        
        if(print_ast_flag)
            print_ast(root); 
        if(error_num == 0){

            translate(root);
        }
    }
    
    fclose(yyin);
}


/* Apre un nuovo scope. Se lo scope è quello di una funzione vi aggiunge 
   eventuali parametri, presenti in param_list, e il tipo di ritorno della funzione 
*/
void scope_enter() {
    current_symtab = create_symtab(current_scope_lvl, current_symtab); 
    current_scope_lvl++;

    if(param_list) { 
        fill_symtab(current_symtab, param_list, -1, PARAMETER); 
        param_list = NULL;
    }

    if(ret_type != -1) {
        insert_sym(current_symtab, "return", ret_type, F_RETURN, NULL, yylineno, line);
        ret_type = -1;
    }
}

/* Chiude uno scope. Eventualmente effettua il print della Symbol Table.
   Verifica che le variabili dichiarate all'interno dello scope siano utilizzate
*/
void scope_exit() {
    if(print_symtab_flag)
        print_symtab(current_symtab);
    check_usage(current_symtab); 
    current_symtab = delete_symtab(current_symtab); 
    current_scope_lvl--;
}



/* Usata per inserire uno o più simboli all'interno della Symbol Table. 
   Usata per 1 - Dichiarazione di variabili
             2 - Dichiarazione di variabili con assegnazione
             3 - Parametri di una funzione
*/
void fill_symtab(struct symlist * syml, struct AstNode *n, enum TYPE type, enum sym_type sym_type){
    /*  syml = puntatore alla ST dello scope corrente
        n = puntatore all'Ast node
        type = tipo di dato (int, float, ...)
        sym_type = tipo di simbolo (in questo caso può essere solo parametro o variabile)
    */
    while(n){      
        if(n -> nodetype == VAR_T) {
            insert_sym(syml, n -> node.var -> name, type, sym_type, NULL, yylineno, line);
            if(n -> node.var -> array_dim){
                /* se è un array (c'è array dim nel nodo ast), modifico il flag nella ST*/
                struct symbol *s = find_sym(syml, n -> node.var -> name);
                s -> array_flag = 1;
            }
        }else if(n -> nodetype == EXPR_T){
            /* Dichiarazioni con assegnazione, in questo caso EXPR_T sarà sempre un'assegnazione */
            insert_sym(syml, n -> node.expr -> l -> node.var -> name, type, sym_type, NULL, yylineno, line);
            if(n -> node.expr -> l -> node.var -> array_dim){
                /* se è un array (c'è array dim nel nodo ast), modifico il flag nella ST*/
                yyerror("invalid array initializer");
                struct symbol *s = find_sym(syml, n -> node.expr -> l -> node.var -> name);
                s -> array_flag = 1;
            }
            /*  controllo sulla correttezza dell'assegnazione */
            eval_expr_type(n);
        }else if(n -> nodetype == DECL_T){
            /* il primo nodo passato sarà sempre di tipo DECL_T da cui verrà estratto type  
            e per essere usato anche nei casi successivi */
            type = n -> node.decl -> type;
            fill_symtab(syml, n -> node.decl -> var, type, sym_type);
        } 

        n = n -> next;
    }
}

/*  Funzione richiamata in caso si usi il flag --help o -h.
    Mostra a schermo l'uso del compilatore 
*/
void print_usage(){
    printf("Usage: ./compiler [options] file  \n");
    printf("options: \n");
    printf(" --help \t Diplay this information. \n");
    printf(" -h \t\t Diplay this information. \n");
    printf(" -s \t\t Print Symbol Table. \n");
    printf(" -t \t\t Print Abstract Syntax Tree. \n");
}