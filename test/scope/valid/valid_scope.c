int b = 2;
void f(int c){
    printf("FUNCTION: local var:%d  \t global var: %d \n", c, b);
    for(int b=0; b<2; b = b+ 1){
        printf("For scope %d \t func variable %d \n", b, c);
    }
}

void main(){
    int a = 1;
    printf("MAIN: local var:%d  \t global var: %d \n", a, b);
    f(3);
}