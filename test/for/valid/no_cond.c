void main(){
    int c;
    
    printf("inserire caso da provare [1-4]: \n");
    scanf("%d", &c);
    
    // ATTENZIONE tutti i casi vanno in loop
    if(c == 1){
    for(int i=0; ; i = i + 1)
        printf("NO_COND loop \n");
    }else if(c == 2){
        for(int i = 0; ;){
            i = i + 1;
            printf("NO_COND & NO_UPDATE loop \n");
        }
    }else if(c == 3){
        int i = 0;
        for(; ;i = i + 1)
            printf("NO_INIT & NO_COND loop \n");
    }else {
        for(;;)
            printf("NO_INIT & NO_COND & NO_UPDATE loop \n");
    }
}