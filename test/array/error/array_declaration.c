void main(){
    int a[-1]; // dimensione negativa
    int b[2-3]; // espressione valutata negativa
    int c[1 > 0]; // operatore non valido 

    float f = 10.2;
    int d[f]; // variabile float
    int i = 1;
    int e[i + f + 1]; // espressione contenente variabile float
    int g[3.2]; // dimesione non float

    int h[3] = 1; // inizializzazione non valida

    printf("%d %d %d %d %d %d %d", a[0], b[0], c[0], d[0], e[0], g[0], h[0]);
}