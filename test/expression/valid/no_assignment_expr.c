void main(){
    int a = 10;
    int b = 10;

    a + b;

    1 + 2;

    // in golang darebbe errore "evaluated but not used", per tradurlo comunque, con lo stesso comportamento del c,
    // traduciamo come _ = expr, (blank identifier) https://golang.org/doc/effective_go#blank
}