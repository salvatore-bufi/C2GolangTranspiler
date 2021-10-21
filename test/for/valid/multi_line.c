void main(){
    int i; // dichiarazione esterna
    for(i = 0; i < 2; i = i + 1){
        printf("i: %d\n", i);
    }

    // dichiarazione nell'init del for
    for(int k = 2; k > 0; k = k - 1){
        int k = 7; // dichiarazione interna allo scope del for
        printf("k: %d \n", k);
    }
}