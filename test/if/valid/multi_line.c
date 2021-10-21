void main(){
    if (1 < 2){
        printf("success \n");
        int a = 0;
        printf("%d \n", a);
    } else{
        printf("fail \n");
    }
    
    if(1 > 0){
        printf("success \n");
        int a = 1;
        printf("%d \n", a);
    }else{
        printf("fail \n");   
    }

    if(1 == 1){
        printf("success \n");
        int a = 2;
        printf("%d \n", a);
    }else{
        printf("fail \n");
    }
    

    if (1 != 2){
        printf("success \n");
        int a = 3;
        printf("%d \n", a);
    } else{
        printf("fail \n");
    }


     if (1 != 2 && 1 < 2){
        printf("success \n");
        int a = 4;
        printf("%d \n", a);
    } else{
        printf("fail \n");
    }

    if (1 != 1 || 1 < 2){
        printf("success \n");
        int a = 5;
        printf("%d \n", a);
    } else{
        printf("fail \n");
    }
    

    if (!(1 == 1)){
        printf("fail \n");
    } else{
        
        printf("success \n");
        int a = 6;
        printf("%d \n", a);
    }
}