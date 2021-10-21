void print_array(int v[], int len){
    printf("\n \narray: \n");
    for(int i = 0; i < len; i = i + 1 ){
        printf("%d \t ", v[i]);
    }
    printf("\n");
}


void bubblesort(int v[], int len) {
    int temp;

    for(int i = 0; i< len-1; i = i + 1) {
        for(int k = 0; k<len-1-i; k = k + 1) {
            if(v[k] > v[k+1]) {
                temp = v[k];
                v[k] = v[k+1];
                v[k+1] = temp;
            }
        }
    }
}


void main(){
    int len;
    int v[20];

    printf("inserire la dimensione del vettore (max 20 elementi) \n");
    scanf("%d",&len);

    if(len > 20 || len < 0){
        printf("dimensione non valida \n");
    } else {
        for(int i = 0; i <  len; i = i + 1){
            printf("inserire l'elemento %d del vettore: \n", i);
            scanf("%d", &v[i]);        
        }
        
        bubblesort(v, len);
        print_array(v, len);
    }
}