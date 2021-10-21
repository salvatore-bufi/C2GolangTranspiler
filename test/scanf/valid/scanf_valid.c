
void main(){
    int a;
    float b;
    double c;

    int v1[1];
    float v2[1];
    double v3[1];
    
    scanf("%d", &a);
    scanf("%f", &b);
    scanf("%lf", &c);

    scanf("%d", &v1[0]);
    scanf("%f", &v2[0]);
    scanf("%lf", &v3[0]);


    printf("printf int: %d\n", a);
    
    printf("printf float: %f\n", b);
    printf("printf double: %f\n", c);

    printf("printf vettori: int %d, \t float %f, \t double %f \n", v1[0], v2[0], v3[0]);
}