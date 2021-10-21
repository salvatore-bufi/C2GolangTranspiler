int i;

void f(){
    i = i + 1;
}

void main(){
    for(i = 0; i < 2; f()){
        printf("i: %d \n", i);
    }
}
