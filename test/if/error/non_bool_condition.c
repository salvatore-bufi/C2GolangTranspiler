int f(){
    return 1;
}

void main(){
    int a = 1;
    
    if(a)
        printf("%d", a);

    if(1)
        printf("%d", 1);

    if(-1)
        printf("%d", -1);

    if(1+1)
        printf("%d", 1+1);
        
    if(1-1)
        printf("%d", 1-1);
    
    if(1*1)
        printf("%d", 1*1);

    if(1/1)
        printf("%d", 1/1);    
    
    if(f())
        printf("%d", 1);

}