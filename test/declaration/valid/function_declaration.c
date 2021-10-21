int f0(){
    printf("funzione f0 int senza argomenti\n");
    return 1;
}

int f1(int a, float b, double c, int d[], float e[], double f[]){
    printf("funzione f1 int con argomenti\n");
    printf("%d %f %f \n", a, b, c);
    printf("%d %f %f \n", d[0], e[0], f[0]);
    return 1;
}

float f2(){
    printf("funzione f2 float senza argomenti \n");
    return 3.5;
}

float f3(int a, float b, double c, int d[], float e[], double f[]){
    printf("funzione f3 float con argomenti\n");
    printf("%d %f %f \n", a, b, c);
    printf("%d %f %f \n", d[0], e[0], f[0]);
    return 1;
}

double f4(){
    printf("funzione f4 double senza argomenti \n");
    return 1;
}

double f5(int a, float b, double c, int d[], float e[], double f[]){
    printf("funzione f4 double con argomenti\n");
    printf("%d %f %f \n", a, b, c);
    printf("%d %f %f \n", d[0], e[0], f[0]);
    return 1;
}

void f6(){
    printf("funzione f6 void senza argomenti \n");
    return ;
}

void f7(int a, float b, double c, int d[], float e[], double f[]){
    printf("funzione f6 void con argomenti\n");
    printf("%d %f %f \n", a, b, c);
    printf("%d %f %f \n", d[0], e[0], f[0]);
}

void main(){
    int a = 1;
    float b = 2.2;
    double c = 3.3;

    int d[1];
    float e[1];
    double f[1];

    d[0] = 4;
    e[0] = 5.5;
    f[0] = 6.6;

    printf("Valore di ritorno di f0 %d \n",f0());
    printf("Valore di ritorno di f1 %d \n",f1(a, b, c, d, e, f));
    printf("Valore di ritorno di f2 %f \n",f2()); 
    printf("Valore di ritorno di f3 %f \n",f3(a, b, c, d, e, f)); 
    printf("Valore di ritorno di f4 %f \n",f4()); 
    printf("Valore di ritorno di f5 %f \n",f5(a, b, c, d, e, f)); 

    f6();
    f7(a, b, c, d, e, f);
}