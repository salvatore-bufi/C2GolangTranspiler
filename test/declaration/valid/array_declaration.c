void main(){
    int a = 1;
    int v1[3 * 4 + 3];
    float v2[ 3 * 6 /2];
    double v3[9 / 3 - a];
    
    v1[0] = 1;
    v2[0] = 2.2;
    v3[0] = 3.3;

    printf("%d %f %f", v1[0], v2[0], v3[0]); 
}