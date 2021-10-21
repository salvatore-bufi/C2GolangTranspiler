void print_vay(int v[], int len){
    printf("\n \narray: \n");
    for(int i = 0; i < len; i = i + 1 ){
        printf("%d \t ", v[i]);
    }
    printf("\n");
}


void  selectionsort(int v[],int len){  
    int min = v[0];

    int i=0, min_index=0, j=0;
    for(i=0; i<len-1; i = i + 1){
        min = v[i];
        min_index = i;
        for(j=i+1; j<len; j = j + 1){
            if(v[j] < min){
                min = v[j];
                min_index = j;
            }
        }        
        v[min_index]=v[i];
        v[i]=min;        
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
        
        selectionsort(v, len);
        print_vay(v, len);
    }
}