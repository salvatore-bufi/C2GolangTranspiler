void main(){
    int i = 0;

    for(;i <2; i = i + 1) // niente init
        printf("NO_INIT i: %d \n", i);

    for(i = 0; i <2;){  // niente update
        printf("NO_UPDATE i: %d \n", i);
        i = i + 1;
    }

    i = 0;
    for(; i < 2 ; ){ // niente init e update
        printf("NO_INIT && NO_UPDATE i: %d \n", i);
        i = i + 1;
    }
}