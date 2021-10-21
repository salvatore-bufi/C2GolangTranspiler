void sum_array(int v1[], int v2[], int len){
    for(int i = 0; i < len; i = i + 1 ){
        printf("%d \n", v1[i] + v2[i] );
    }
}

void diff_array(int v1[], int v2[], int len){
    for(int i = 0; i < len; i = i + 1 ){
        printf("%d \n", v1[i] - v2[i] );
    }
}

void mul_array(int v1[], int v2[], int len){
    for(int i = 0; i < len; i = i + 1 ){
        printf("%d \n", v1[i] * v2[i] );
    }
}

void div_array(int v1[], int v2[], int len){
    for(int i = 0; i < len; i = i + 1 ){
        printf("%d \n", v1[i] / v2[i] );
    }
}

void main(){ 
    int a[2];
    int b[2];

    a[0] = 8;
    a[1] = 2;

    b[0] = 2;
    b[1] = 8;


    sum_array(a, b, 2);
    diff_array(a, b, 2);
    mul_array(a, b, 2);
    div_array(a, b, 2);

}