void f(){
    int a = 10;
    printf("%d", a);

    if(a == 10){
        int b = 20;
        printf("%d", b);
    }

    printf("%d", b); // fuori dallo scope dell'if

    for(int i = 0; i < 2; i = i + 1){
        printf("%d", i);
    }

    printf("%d", i);
}

void main(){
    printf("%d", a);  // fuori dallo scope della funzione f
    printf("%d", c); // variabile globale usata prima della dichiarazione
}

int c = 1;

