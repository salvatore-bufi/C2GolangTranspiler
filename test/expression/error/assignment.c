void main(){
    int a;
    float b;

    // costanti troncate in assegnazione

    a = 3.5;

    a = 3 * 2 - 6.5;

    // bool assegnato a float

    b = 1 > 2;
    
    // operatori arimetici non definiti su bool

    b = (1 < 3) + (4 > 2);

    b = (1 > 4) * (3 != 6);

    b = (2 == 4) - (7 < 2);

    b = (1 > 4) / (3 != 6);

    // operatori di confronto non definiti su bool
    
    b = (1 < 3) > (4 > 2);

    b = (1 > 4) < (3 != 6);

    b = (2 == 4) >= (7 < 2);

    b = (1 < 3) <= (4 > 2);

    printf(" %d %f ", a, b);
    
}