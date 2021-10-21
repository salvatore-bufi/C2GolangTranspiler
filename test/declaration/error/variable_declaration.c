void main(){
    int a0 = 10.2; // troncamento di costante
    int a1 = 10 > 1; // bool assegnato ad int
    int a2 = 1 > 0 && 0 < 1; // bool assegnato ad int

    float b0 = 10 > 1; // bool assegnato ad float
    float b1 = 1 > 0 && 0 < 1; // bool assegnato ad float

    double c0 = 10 > 1; // bool assegnato ad double
    double c1 = 1 > 0 && 0 < 1; // bool assegnato ad double

    int a = b0; // variabile float assegnato ad int
    float b = c0; // variabile double assegnato ad float
    double c = a0; // variabile int assegnato ad float 

    printf("%d %d %d %d %f %f %f %f %f %f", a, a0, a1, a2, b, b0, b1, c, c0, c1);
}