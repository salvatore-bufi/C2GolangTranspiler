#include "semantic.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include "symtab.h"
#include "tree.h"
#include "translate.h"

int div_by_zero; // flag che indica se ho una divisione per in eval_array_dim

/* Controlla la correttezza della dimensione o indice di un array */
void check_array(struct AstNode *dim){
    /*  dim = dimensione o indice dell'array */
    if(check_array_dim(dim)){
        div_by_zero = 0;
        if (eval_array_dim(dim) < 0){
            yyerror("cannot use negative array index or size");
        }
    }
}


/* Controlla se l'espressione è costante:
        ritorna 1: se l'espressione è costante intera
        ritorna 0: altrimenti
        errori: variabili,valori e funzioni con ritorno di tipo non intero 
                operatori che restituiscono un valore booleano */
int check_array_dim(struct AstNode *expr){
    /*  expr = espressione che rappresenta la dimensione o indice di un array */
    switch(expr -> nodetype){
        case VAL_T:
            if(expr -> node.val -> val_type == FLOAT_T){
                    yyerror("invalid array index or size, non-integer len argument");
                    return 0;
                }
            return 1;
        case VAR_T: ;
            struct symbol *var = find_symtab(current_symtab, expr -> node.var -> name);
            if(var != NULL && var -> type != INT_T)
                yyerror("invalid array index or size, non-integer len argument");
            return 0;
        case FCALL_T: ;
            struct symbol *f = find_symtab(current_symtab, expr -> node.fcall -> name);
            if(f != NULL && f -> type != INT_T)
                yyerror("invalid array index or size, non-integer len argument");
            return 0;
        case EXPR_T: ;
            int l = 1;
            int r = 1;
            switch(expr -> node.expr -> expr_type){
                case NOT_T:
                case AND_T:
                case OR_T:
                case G_T:
                case GE_T:
                case L_T:
                case LE_T:
                case EQ_T:
                case NE_T:
                case ASS_T:
                    yyerror(error_string_format("invalid array index or size, cannot use operator " BOLD "%s" RESET, convert_expr_type(expr -> node.expr -> expr_type)));
                    return 0;
            }

            if(expr -> node.expr -> l)
                l = check_array_dim(expr -> node.expr -> l);

            if(expr -> node.expr -> r)
                r = check_array_dim(expr -> node.expr -> r);

            return l && r;
    } 
}


/* funzione richiamata solo se l'espressione è costante intera.
    Controlla che la dimensione/indice sia un numero intero >= 0 */
int eval_array_dim(struct AstNode *expr){
    /* Nota:    go restituisce un intero anche se il risultato è un numero decimale, l'importante è 
                che si lavori con numeri int. 
                esempio int a[3/2] non da errore anche se è float il risultato, lo approssima ad a[1] */
    /*  expr = espressione che rappresenta la dimensione o indice di un array */
    switch(expr -> nodetype){
        int l, r;
        case VAL_T:
            if(expr -> node.val -> val_type == INT_T)
                return atoi(expr -> node.val -> string_val);
        case EXPR_T:
            if(expr -> node.expr -> l)
                l = eval_array_dim(expr -> node.expr -> l);

            if(expr -> node.expr -> r)
                r = eval_array_dim(expr -> node.expr -> r);

            if(div_by_zero)
                return 0;
            
            switch(expr -> node.expr -> expr_type){
                case ADD_T:
                    return l + r;
                case SUB_T:
                    return l - r;
                case MUL_T:
                    return l * r;
                case DIV_T:
                    if(r == 0){
                        div_by_zero = 1;
                        return 0;
                    }
                    return l / r;
                case NEG_T:
                    return - r;
                case PAR_T:
                    return r;
            }
    }
}

/* Calcola il risultato di un espressione costante */
float eval_constant_expr(struct AstNode *expr){
    /*  expr = espressione da valutare */
    switch(expr -> nodetype){
        float l, r;
        case VAL_T:
            return atof(expr -> node.val -> string_val);
        case EXPR_T:
            if(expr -> node.expr -> l)
                l = eval_constant_expr(expr -> node.expr -> l);

            if(expr -> node.expr -> r)
                r = eval_constant_expr(expr -> node.expr -> r);

            switch(expr -> node.expr -> expr_type){
                case ADD_T:
                    return l + r;
                case SUB_T:
                    return l - r;
                case MUL_T:
                    return l * r;
                case DIV_T:
                    return l / r;
                case NEG_T:
                    return - r;
                case PAR_T:
                    return r;
            }
    }
}

/* Controlla se un espressione costante presenta una divisione per zero */
void check_division(struct AstNode *expr){
    /*  expr = espressione da valutare */
    if(eval_expr_type(expr).kind == N){
        if(eval_constant_expr(expr) == 0){
            yyerror("division by zero");
        }
    }
}

/* Valuta il tipo di un espressone e se tale espressione è costante o meno 
    (cioè formata da soli numeri)
    Restituisce un complex_type con kind = N se l'espressione è costante, 
    altrimenti kind = V. */
struct complex_type eval_expr_type(struct AstNode *expr){
    /*  expr = espressione da valutare
        complex_type => {   kind = V o N 
                            type = INT_T,... o ERROR_T } */
    switch(expr -> nodetype){
        struct complex_type res;
        case VAL_T:
            res.kind = N;
            res.type = expr -> node.val -> val_type;
            return res;
        case VAR_T: ;
            struct symbol *var = find_symtab(current_symtab, expr -> node.var -> name );
            res.kind = V;
            if(var){
                res.type = var -> type;
                // controlliamo se la variabile è un vettore
                if( var -> array_flag){
                    // controlliamo se accediamo ad un elemento del vettore
                    // esempio non possiamo fare a + 10, se a è un array
                    if(expr -> node.var -> array_dim)
                        res.type = var -> type;
                    else{
                        res.type = ERROR_T;
                        yyerror(error_string_format("cannot use a type " BOLD "[]%s" RESET " as type " BOLD "%s" RESET, convert_var_type(var -> type), convert_var_type(var -> type)));
                    }
                } else {
                    // controlliamo se usiamo l'indicizzazione su una variabile che non sia un array
                    if(expr -> node.var -> array_dim){
                     yyerror(error_string_format("invalid operation: " BOLD "%s[%s]" RESET " (type %s does not support indexing)", var -> name, expr -> node.var -> array_dim -> node.val -> string_val, convert_var_type(var -> type)));
                     res.type = ERROR_T;
                    }
                }
            }else
                res.type = ERROR_T;
            return res;
        case FCALL_T: ;
            struct symbol *fcall = find_symtab(current_symtab, expr -> node.fcall -> name );
            res.kind = V;
            if(fcall){
                res.type = fcall -> type;
            }else
                res.type = ERROR_T;
            return res;
        case EXPR_T: ;
            struct complex_type l;
            struct complex_type r;

            if(expr -> node.expr -> l){
                l = eval_expr_type(expr -> node.expr -> l);
                if(l.type == ERROR_T)
                    return l;
            }
            
            if(expr -> node.expr -> r){
                r = eval_expr_type(expr -> node.expr -> r);
                if(r.type == ERROR_T)
                    return r;
            }

            /* operatori confronto */
            if(expr -> node.expr -> expr_type >= G_T && expr -> node.expr -> expr_type <= NE_T){
                res =  eval_comparison_op_type(l, r, expr -> node.expr -> expr_type);
                return res;
            }

            /* operatori aritmetici */
            if(expr -> node.expr -> expr_type > ASS_T && expr -> node.expr -> expr_type <= MUL_T){
                res = eval_arithmetic_op_type(l, r);
                return res;
            } else if (expr -> node.expr -> expr_type == ASS_T){
                res = eval_ass_op(l, r, expr -> node.expr -> r);
                return res;
            }

            /* per parentesi o operatore meno unario */
            if(expr -> node.expr -> expr_type == PAR_T || expr -> node.expr -> expr_type == NEG_T){
                // parentesi o op. meno unario non cambiano il tipo di r
                return r;
            }

            /* operatori logici */
            // gli operatori logici sono definiti solo su operandi booleani
            if(expr -> node.expr -> expr_type == NOT_T){
                if(r.type == BOOL_T){
                    res.kind = V;
                    res.type = BOOL_T;
                    return res;
                }else{
                    res.kind = V;
                    res.type = ERROR_T;
                    yyerror(error_string_format("invalid operation: operator " BOLD "%s" RESET " defined only on bool", convert_expr_type(expr -> node.expr -> expr_type)));
                    return res;
                }
            }

            if(expr -> node.expr -> expr_type == AND_T || expr -> node.expr -> expr_type == OR_T ){
                if(l.type == BOOL_T && r.type == BOOL_T){
                    res.kind = V;
                    res.type = BOOL_T;
                    return res;
                }else{
                    res.kind = V;
                    res.type = ERROR_T;
                    yyerror(error_string_format("invalid operation: operator " BOLD "%s" RESET " defined only on bool", convert_expr_type(expr -> node.expr -> expr_type)));
                    return res;
                }
            } 
    }
}

/*  Valuta il tipo restituito da un operatore di confronto 
    in caso i tipi degli operandi non sono compatibili restituisce errore */
struct complex_type eval_comparison_op_type(struct complex_type l, struct complex_type r, enum EXPRESSION_TYPE expr_type){
    /*  l = primo operando
        r = secondo operando
        expr_type = operatore 
    */
    struct complex_type res;

    // gli unici operatori di confronto definiti sui booleni sono == e != 
    // es: 1 > 2 == 3 < 4 -> ok    -----  1 > 2  <   3 > 4 -> no
    if(expr_type != NE_T && expr_type != EQ_T && (l.type == BOOL_T || r.type == BOOL_T)){
        yyerror("invalid operation: operation not defined on bool");
        res.type = ERROR_T;
        res.kind = V;
        return res;
    }

    // è possibile confrontare solo variabili dello stesso tipo
    if(l.kind == V && r.kind == V){
        if(l.type == r.type){
            res.kind = V;
            res.type = BOOL_T;
            return res;
        }else{
            res.type = ERROR_T;
            res.kind = V;
                yyerror("invalid operation: mysmatched types");
            return res;
        }
    }

    // è possibile confrontare variabili e numeri solo se il numero è rappresentabile nel tipo della variabile
    // float a; ...   a < 1 ==> ok   ----   int a; ...  a > 2.3 ==> no
    if(l.kind == V && r.kind == N){
        if(l.type >= r.type){
            res.kind = V;
            res.type = BOOL_T;
            return res;
        }else{
            res.type = ERROR_T;
            res.kind = V;
                yyerror("constant truncated");
            return res;
        }
    }

    if(l.kind == N && r.kind == V){
        if(l.type <= r.type){
            res.kind = V;
            res.type = BOOL_T;
            return res;
        }else{
            res.type = ERROR_T;
            res.kind = V;
                yyerror("constant truncated");
            return res;
        }
    }

    // è possibile confrontare qualsiasi costante numerica
    if(l.kind == N && r.kind == N){
        res.kind = V;
        res.type = BOOL_T;
        return res;
    }
}


/*  Valuta il tipo restituito da un operatore aritmetico 
    in caso i tipi degli operandi non sono compatibili restituisce errore */
struct complex_type eval_arithmetic_op_type(struct complex_type l, struct complex_type r){
    /*  l = primo operando
        r = secondo operando
    */

    struct complex_type res;

    // gli operatori aritmetici non sono definiti sui booleani
    if(l.type == BOOL_T || r.type == BOOL_T){
        yyerror("invalid operation: operation not defined on bool ");
        res.type = ERROR_T;
        res.kind = V;
        return res;
    }
    
    // è possibile effetuare operazioni aritmetiche solo se le variabili sono dello stesso tipo
    if(l.kind == V && r.kind == V){
        if(l.type == r.type){
            return l;
        }
        else{
            res.type = ERROR_T;
            res.kind = V;
                yyerror("invalid operation: mysmatched types");
            return res;
        }
    }

    // è possibile effetuare operazioni aritmetiche tra variabili e numeri solo se il numero è rappresentabile nel tipo della variabile
    if(l.kind == V && r.kind == N){
        if(l.type >= r.type){
            return l;
        }else{
            res.type = ERROR_T;
            res.kind = V;
            yyerror("constant truncated");
            return res;
        }
    }

    if(l.kind == N && r.kind == V){
        if(l.type <= r.type){
            return r;
        }else{
            res.type = ERROR_T;
            res.kind = V;
            yyerror("constant truncated");
            return res;
        }
    }

    // è sempre possibile effettuare operazioni aritmetiche tra costanti numeriche
    if(l.kind == N && r.kind == N){
        if(l.type <= r.type){
            return r;
        }else{
            return l;
        }
    }
}

/*  Valuta il tipo restituito da un operatore di assegnazione 
    in caso i tipi degli operandi non sono compatibili restituisce errore */
struct complex_type eval_ass_op(struct complex_type l, struct complex_type r, struct AstNode *r_node){
    /*  l = primo operando (tipo della variabile a cui assegnare l'espressione)
        r = secondo operando (tipo dell'espressione da assegnare)
        r_node = espressione da assegnare
        es: int a = 1 + 2; ===> r = a
                                l = tipo complesso restituito da 2+1
                                r_node = 2 + 1
        */
        
    struct complex_type res;
    
    /*  se l'espressione da assegnare non è costante 
        i due operandi devono essere dello stesso tipo */
    if(r.kind == V){
        if(l.type == r.type)
            return l;
        else{
            res.type = ERROR_T;
            res.kind = V;
            yyerror("invalid operation: mysmatched types in assignment");
            return res;
        }
    }
    
    /*  se l'espressione r da assegnare a l è costante*/
    if(r.kind == N){
        if(r.type != INT_T && l.type == INT_T){
            /*  se la variabile a cui assegnare l'espressione è intera, mentre
                l'espressione è costante ma non intera devo controllare se ho un troncamento
                esempio: int a = 10.2 + 1; -> troncamento (ERROR_T)
                         int a = 10.2 + 0.8 -> non ho troncamento (OK)
            */
            float result = check_const_truncated(r_node);
            if(result != (int)result)
                res.type = ERROR_T;
            else 
                res.type = INT_T;
            res.kind = V;
            return res;
        }else if(l.type >= r.type){
            /* controlliamo se il tipo dell'espressione è rappresentabile nel tipo della variabile */
            return l;
        }else{
            res.type = ERROR_T;
            res.kind = V;
            yyerror("constant truncated in assignment");
            return res; 
        }
    }
}

/*  Effettua controlli sulle chiamate di funzioni */
void check_fcall(char *name, struct AstNode *args) {
    /*  name = nome della funzione
        args = puntatore alla lista di argomenti */
        
    struct symbol *s = find_symtab(current_symtab, name); 

    if(!s){
        // controlla che il simbolo sia stato dicharato
        yyerror(error_string_format("function " BOLD "%s" RESET " undeclared", name));
    }else if(s -> sym_type != FUNCTION){
        // controlla che il simbolo sia effettivamente una funzione
        yyerror(error_string_format("called object " BOLD "%s" RESET " is not a function", name));
    }else{
        struct AstNode *a = args;
        struct AstNode *p = s -> pl; 
        
        // controllo sulla coerenza del numero e tipo di argomenti passati alla funzione
        while( a && p ){
            if(a -> nodetype == VAR_T){
                // se l'agomento è una variabile
                struct symbol *arg = find_symtab(current_symtab, a -> node.var -> name);

                // controlliamo che sia stata dichiarata
                if(arg == NULL)
                    break;

                if(arg -> type == p -> node.decl -> type){
                    // controllo sugli array
                    if(arg -> array_flag){ 
                        // se a è un array ed f si aspetta un array ma gli passiamo un elemento      
                        // esempio:  f(a[1]) ma f è definita int f(int a[]){...}                
                        if(a -> node.var -> array_dim && p -> node.decl -> var -> node.var -> array_dim){
                            yyerror(error_string_format("cannot use " BOLD "%s" RESET " (type " BOLD "%s" RESET ") as type " BOLD "[]%s" RESET " in argument to " BOLD "%s" RESET,
                                                        arg -> name, convert_var_type(arg -> type), convert_var_type(p -> node.decl -> type), name));
                        // se a è un array ed f si aspetta una variabile ma gli passiamo un array
                        // esempio: f(a) ma f è definita int f(int a){...}
                        }else if(!a -> node.var -> array_dim && !p -> node.decl -> var -> node.var -> array_dim){
                            yyerror(error_string_format("cannot use " BOLD "%s" RESET " (type " BOLD "[]%s" RESET ") as type " BOLD "%s" RESET " in argument to " BOLD "%s" RESET,
                                                        arg -> name, convert_var_type(arg -> type), convert_var_type(p -> node.decl -> type), name));
                        }
                    }else{
                        // se a non è un array ma f si aspetta un array
                        // esempio: int a; f(a) ma f è definita int f(int a[]){...}
                        enum TYPE type = eval_expr_type(a).type;
                        if(type != ERROR_T && p -> node.decl -> var -> node.var -> array_dim){
                            yyerror(error_string_format("cannot use " BOLD "%s" RESET " (type " BOLD "%s" RESET ") as type " BOLD "[]%s" RESET " in argument to " BOLD "%s" RESET,
                                                        arg -> name, convert_var_type(type), convert_var_type(p -> node.decl -> type), name));
                        }
                    }
                }else{
                    // se tipo di argomento e parametro sono diversi
                    yyerror(error_string_format("type mismatch in function " BOLD "%s" RESET , name));
                }
            }else{
                // se l'argomento è un espressione
                struct complex_type res = eval_expr_type(a);
                enum TYPE type = res.type;

                // controllo sul troncamento della costante
                // esempio: int f(int a){...} e chiamiamo f(1.2); -> troncamento della costante
                if(res.kind == N && res.type != INT_T && p -> node.decl -> type == INT_T){
                    check_const_truncated(a);
                }else if(type != ERROR_T && res.kind == V && type != p -> node.decl -> type){
                    // tipo diverso tra argomento e parametro
                    yyerror(error_string_format("type mismatch in function " BOLD "%s" RESET , name));
                }else{
                    if(type != ERROR_T && p -> node.decl -> var -> node.var -> array_dim)
                        // la funzione si aspetta un array ma l'argomento è un espressione
                        yyerror(error_string_format("cannot use " BOLD "%s" RESET " (type " BOLD "%s" RESET ") as type " BOLD "[]%s" RESET " in argument to " BOLD "%s" RESET,
                                                        a -> node.var -> name, convert_var_type(type), convert_var_type(p -> node.decl -> type), name));
                }
            }
            a = a -> next;
            p = p -> next; 
        }

        // numero di argomenti diverso dal numero di parametri
        if ( a != p  && a == NULL){
            yyerror(error_string_format("too few arguments to function " BOLD "%s" RESET, name));
        }
        else if ( a != p && p == NULL){
            yyerror(error_string_format("too many arguments to function " BOLD "%s" RESET, name));
        }
    }
}

/* Controlla se il tipo ritornato dalla funzione coincide con quello della sua dichiarazione */
void check_return(struct AstNode *expr) {
    /*  expr = espressione nell'istruzione return (return expr) 
        è NULL se abbiamo return ; */
    struct symbol* s = find_symtab(current_symtab, "return"); 
    
    if(expr){
        if( s -> type == VOID_T){
            /* funzione void ma ritorniamo espressione */
            yyerror(BOLD "return" RESET " with a value, in function returning void");
            yynote(error_string_format("function declaration was here"), s -> lineno, s -> line);
        } else {
            /* controllo sul tipo dell'espressione */
            struct complex_type ret_expr = eval_expr_type(expr);

            if(ret_expr.kind == N && ret_expr.type != INT_T){
                if( s -> type == INT_T){
                    /* int f(){... return 3.1 + 2.9; } */
                    check_const_truncated(expr);
                }
            }else if(ret_expr.kind == N){
                if(s -> type < ret_expr.type) {
                    /*  float f() { ... return 1; } -> ok 
                        int f() { ... return 1.2; } -> errore */
                    yyerror(error_string_format("constat truncate to " BOLD "%s" RESET, convert_var_type(s -> type)));
                }
            }else{
                /* se l'espressione non è costante i tipi devono essere uguali */
                if(s -> type != ret_expr.type) {
                    yyerror(error_string_format("cannot use a " BOLD "%s" RESET " as type " BOLD "%s" RESET " in return argument", convert_var_type(ret_expr.type), convert_var_type(s -> type)));
                    yynote(error_string_format("function declaration was here"), s -> lineno, s -> line);
                }
            }
        }
    }else{
        /* se l'espressione è NULL e il tipo della funzione non è void */
        if(s -> type != VOID_T) {
            yyerror(BOLD "return" RESET " with no value, in function returning non-void");
            yynote(error_string_format("function declaration was here"), s -> lineno, s -> line);
        }
    }
}

/* Controlla se ho return all'interno di una funzione */
void check_func_return(enum TYPE type, struct AstNode *code){
    /*  type = tipo dichiarato della funzione
        code = puntatore al codice all'interno della funzione */
    
    int ret = 0;

    if(type != VOID_T){ 
        for(struct AstNode *tmp = code; tmp != NULL; tmp = tmp -> next){
            // controllo se l'ultimo statement della funzione è un IF o un RETURN
            if(tmp -> next == NULL && tmp -> nodetype == IF_T){     
                ret = check_if_return(tmp); // se è un IF devo controllare che ci sia return nei branch dell'if
            }
            // se l'ultimo statement è un return è OK perché ho il ritorno alla fine della funzione
            if(tmp -> next == NULL && tmp -> nodetype == RETURN_T){ 
                ret = 1;
            }
        }

        if(!ret){
            yyerror("missing return at end of function");
        }
    }
}

/* Controllo la presenza di return all'interno del costrutto if-else */
int check_if_return(struct AstNode *tmp){
    /* tmp =  costrutto if */
    int ret;

    // controllo sul body dell'if
    for(struct AstNode *body = tmp -> node.ifn -> body; body != NULL; body = body -> next){
        // se l'ultima istruzione è un if, controllo ricorsivamente
        if(body -> next == NULL && body -> nodetype == IF_T)
            ret = check_if_return(body);
        // se l'ultima istruzione è un return, abbiamo return nel branch if
        if(body -> next == NULL && body -> nodetype == RETURN_T)
            ret = 1;
        // se l'ultima istruzione non è un return, non abbiamo return nel branch if
        if(body -> next == NULL && body -> nodetype != RETURN_T && body -> nodetype != IF_T)
            ret = 0;
    }

    // controllo il body dell'else solo se ho return nell'if
    if(ret){
        // se non c'è l'else, non ho return nel branch else
        if(tmp -> node.ifn -> else_body == NULL){
            ret = 0;
        }else{
            for(struct AstNode *else_body = tmp -> node.ifn -> else_body; else_body != NULL; else_body = else_body -> next){
                // se l'ultima istruzione è un if, controllo ricorsivamente
                if(else_body -> next == NULL && else_body -> nodetype == IF_T)
                    ret = check_if_return(else_body);
                // se l'ultima istruzione è un return, abbiamo return nel branch else
                if(else_body -> next == NULL && else_body -> nodetype == RETURN_T)
                    ret = 1;
                // se l'ultima istruzione non è un return, non abbiamo return nel branch else
                if(else_body -> next == NULL && else_body -> nodetype != RETURN_T && else_body -> nodetype != IF_T) 
                    ret = 0;
            }
        }
    }
    return ret;
}

/* Controlla se il tipo della condizione dell'if e del for è booleano */
void check_cond(enum TYPE type){
    if(type != BOOL_T && type != ERROR_T){
        yyerror("non-bool expression used as condition");
    }
}

/* Controlla se vi è il troncamento di costanti in un espressione */
float check_const_truncated(struct AstNode * expr ){
    /* expr = expressione
    funzione che valuta se l'expr può essere assegnata ad un intero
    es int a = 12.0 -> ok
       return 9.0 (f int) -> ok
       return 9.2 o int a = 12.1 -> error
    */
    float result = eval_constant_expr(expr);

    if(result != (int)result)
        yyerror(error_string_format("constant " BOLD "%f" RESET " truncated to integer", result));
    return result;
}

/* Restituisce la lista degli specificatori di formato all'interno della format string */
struct AstNode* extract_format_specifier(struct AstNode *format_string, enum FUNC_TYPE f_type){
    /*  format_string = nodo Ast da cui estrarre la format string
        f_type = indica la funzione: scanf o printf */
    char *s = format_string -> node.val -> string_val;
    struct AstNode *tmp = NULL; // puntatore all'ultimo elemento creato nella lista degli specificatori di tipo
    struct AstNode *format_specifier = NULL; // puntatore all'inizio della lista degli specificatori di tipo

    for(int i = 0; i < strlen(s) - 1; i++) {
        if(s[i] == '%'){
            switch(s[i+1]){
                char *c;
                case 'd':
                case 'i':
                    c = malloc(sizeof(char)+ 1);
                    strncpy(c, &s[i+1], 1);
                    if(format_specifier){
                        tmp = append_AstNode(tmp, new_value(VAL_T, INT_T, c)); 
                    }else{
                        format_specifier = new_value(VAL_T, INT_T, c);
                        tmp = format_specifier;
                    }
                    break;
                case 'f':
                case 'F':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                    c = malloc(sizeof(char)+ 1);
                    strncpy(c, &s[i+1], 1);
                    if(f_type == PRINTF_T){
                        if(format_specifier){
                            tmp = append_AstNode(tmp, new_value(VAL_T, DOUBLE_T, c));
                        }else{
                            format_specifier = new_value(VAL_T, DOUBLE_T, c);
                            tmp = format_specifier;
                        }
                    }else{
                        if(format_specifier){
                            tmp = append_AstNode(tmp, new_value(VAL_T, FLOAT_T, c));
                        }else{
                            format_specifier = new_value(VAL_T, FLOAT_T, c);
                            tmp = format_specifier;
                        }
                    }
                    break;
                case 'l':
                    if(i+2 < strlen(s) && s[i+2] == 'f'){
                        if(format_specifier){
                            tmp = append_AstNode(tmp, new_value(VAL_T, DOUBLE_T, "lf"));
                        }else{
                            format_specifier = new_value(VAL_T, DOUBLE_T, "lf");
                            tmp = format_specifier;
                        }
                    }
                    break;
                case '%':
                    i++; // per non riconoscere il secondo % come specificatore di tipo: "%% "
                    break; // %% serve per stampare %
                case ' ':
                    yywarning("conversion lacks type of format string");
                    break;
                default:
                    // se ho un % non seguito da un tipo riconosciuto
                    c = malloc(sizeof(char)+ 1);
                    strncpy(c, &s[i+1], 1);
                    yywarning(error_string_format("unknown conversion type character " BOLD "%s" RESET " in format string",  c)); 
            }
        }
    }

    return format_specifier;
}

/* Controlla la coerenza tra specificatori di tipo e argomenti delle funzioni printf e scanf */
void check_format_string(struct AstNode *format_string, struct AstNode *args, enum FUNC_TYPE f_type){
    /* format_string = puntatore ast al nodo da cui estrarre la format string
        args = argomenti delle funzioni
        f_type = indica la funzione: scanf o printf */
    struct AstNode *fs = extract_format_specifier(format_string, f_type);
    struct AstNode *a = args;

    // in golang è usato solo %d per gli interi
    format_string -> node.val -> string_val = replace_format(format_string -> node.val -> string_val, "%i", "%d");

    if (f_type == SCANF_T){
        // in golang anche per scanf usiamo %f per i double
        format_string -> node.val -> string_val = replace_format(format_string -> node.val -> string_val, "%lf", "%f");
        while(a && fs){
            int type = eval_expr_type(a).type;

            if(!a -> node.var -> by_reference){
                // errore se non usiamo l'operatore di referenziazione nello scanf
                yywarning(error_string_format("format %" BOLD "%s" RESET " expects argument of type " BOLD "%s *" RESET ", but argument " BOLD "%s" RESET " has type " BOLD "%s" RESET, 
                        fs -> node.val -> string_val, convert_var_type(fs ->node.val -> val_type), a -> node.var -> name, convert_var_type(type)));
            }else if(type != fs -> node.val -> val_type && type != ERROR_T){
                // tipi diffenti tra argomento e format specifier
                yywarning(error_string_format("format %" BOLD "%s" RESET " expects argument of type " BOLD "%s *" RESET ", but argument " BOLD "%s" RESET " has type " BOLD "%s *" RESET, 
                        fs -> node.val -> string_val, convert_var_type(fs ->node.val -> val_type), a -> node.var -> name,  convert_var_type(type))); // errore o warning?*/
            } 
            a = a -> next;
            fs = fs -> next;
        }
    } else if ( f_type == PRINTF_T){
        while(a && fs){
            int type = eval_expr_type(a).type;

            // tipi differenti tra argomento e format specifier
            // non possiamo fare type != fs ... -> val_type perché %f è usato per float e double
            if(fs -> node.val -> val_type == INT_T && type != fs -> node.val -> val_type && type != ERROR_T){
                yywarning(error_string_format("format %" BOLD "%s" RESET " expects argument " BOLD "%s" RESET " of type " BOLD "%s" RESET ", but argument has type " BOLD "%s" RESET, 
                        fs -> node.val -> string_val, convert_var_type(fs ->node.val -> val_type), a -> node.var -> name, convert_var_type(type))); // errore o warning?*/
            } else if ( type == INT_T && fs -> node.val -> val_type != INT_T){
                 yywarning(error_string_format("format %" BOLD "%s" RESET " expects argument " BOLD "%s" RESET " of type " BOLD "%s" RESET ", but argument has type " BOLD "%s" RESET, 
                        fs -> node.val -> string_val, convert_var_type(fs ->node.val -> val_type), a -> node.var -> name, convert_var_type(type)));
            }

            a = a -> next;
            fs = fs -> next;
        }
    }

    // differente numero di argomenti e format specifier
    if ( a != fs  && a == NULL){
            yywarning("too few arguments to format string");
            while(fs) {
                yywarning(error_string_format("format %" BOLD "%s" RESET " expects a matching " BOLD "%s" RESET " argument", 
                        fs -> node.val -> string_val, convert_var_type(fs ->node.val -> val_type))); // errore o warning?*/
                fs = fs -> next;
            }
        }
    else if ( a != fs && fs == NULL){
            yywarning("too many arguments to format string");
        }
}

/* Controlla se un espressione è usata come statement */
struct AstNode * check_expr_statement(struct AstNode *expr){
    /* expr = espressione da controllare */
    if( expr -> nodetype != FCALL_T){
        /* expr; ======>   _ = expr; */
        return new_expression(EXPR_T, ASS_T, new_variable(VAR_T, "_", NULL), expr);
    }
    return expr;
}

/* Controlla che il main non abbiamo argomenti e sia di tipo void */
void check_main(struct AstNode *fdecl){
    /* fdecl = nodo dichiarazione della funzione */
    if (!strcmp(fdecl -> node.fdef -> name, "main" )){
        main_flag = 1;
        if( fdecl -> node.fdef -> ret_type != VOID_T )
            yyerror("function main must have no return values");
        if( fdecl -> node.fdef -> params)
            yyerror("function main must have no arguments");
    }
}

/* All'utilizzo di una variaible, controlla se è stata dichiarata e ne modifica il flag used_flag */
void check_var_reference(struct AstNode *var){
    struct symbol *s = NULL;
    s = find_symtab(current_symtab, var -> node.var -> name);

    if(!s){ 
        // se non è stata dichiarata errore
        yyerror(error_string_format("variable " BOLD "%s" RESET " undeclared", var -> node.var -> name));
    }else {
        // se esiste impostiamo il flag di reference = 1
        s -> used_flag = 1;
    }
}

/* Modifica parte di una stringa */
char* replace_format(char *string, char *old, char *new){
  /* string = stringa da modificare
    old = parte della stringa da modificare
    new = con cosa sostituire */
    char *tmp = strdup(string);
    char *result = malloc(strlen(tmp) + 1);
    
    /* p punta alla prima occorrenza della stringa da sostituire */
    char *p = strstr(tmp, old);
    
    while(p){
      *p = '\0'; // aggiungo il terminatore di stringa
      strcat(result, tmp); // concateno fino al terminatore di stringa
      strcat(result, new); // concateno la stringa da sostituire
      tmp = p + strlen(old); // faccio puntare tmp dopo la prima occorrenza della stringa sostituita
      p = strstr(tmp, old); // faccio puntare p alla successiva occorrenza della stringa da sostituire
    }
    
    strcat(result, tmp);
    return result;
}