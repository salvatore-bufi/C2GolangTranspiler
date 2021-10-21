void main(){
    int a = 1;
    float b = 2.2;
    double c = 3.3;

    int v1[1];
    float v2[1];
    double v3[1];
    
    v1[0] = 11;
    v2[0] = 22.2;
    v3[0] = 33.3;

    printf("printf int: %d /t %i \n", a, a);
    
    printf("printf float: %f \t %F \t %e \t %E \t %g \t %G\n", b, b, b, b, b, b);
    printf("printf double: %f \t %F \t %e \t %E \t %g \t %G\n", c, c, c, c, c, c);

    printf("printf vettori: int %d, \t float %f, \t double %f \n", v1[0], v2[0], v3[0]);

    printf("printf percentuale: %%f \n");
}