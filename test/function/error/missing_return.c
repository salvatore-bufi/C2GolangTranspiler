int f0(){
    printf("no return \n");
}

int f1(){
    if(1 > 0){
        return 1;
    }
}

int f2(){
    if(1 > 0){
        if(2 > 1){
            return 1;
        }
    }else{
        return 0;
    }
}