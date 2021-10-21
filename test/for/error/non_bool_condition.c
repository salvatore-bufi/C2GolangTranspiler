int f(){
    return 1;
}

void main(){
    for(int i = 0; i; i = i + 1)
        printf("error");
    

    for(int i= 0; 1; i = i + 1)
        printf("error");

    for(int i= 0; -1; i = i + 1)
        printf("error");

    for(int i= 0; 1 + 1; i = i + 1)
        printf("error");

    for(int i= 0; 1 - 1; i = i + 1)
        printf("error");

    for(int i= 0; 1 * 1; i = i + 1)
        printf("error");

    for(int i= 0; i / 1; i = i + 1)
        printf("error");

    for(int i= 0; f(); i = i + 1)
        printf("error");
}