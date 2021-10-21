void f(int a){
    printf("%d \n", a);
}

int f1(){
    return 1;
}

int f2(){
    int a = f1();
    return a;
}

void main(){
    int a = 1, b = 1;

    f(a+b);
    f(a-b);
    f(a*b);
    f(a/b);
    f(f2());
}

