package main 

import "fmt" 

func f0() int  {
	fmt.Printf("funzione f0 int senza argomenti\n")
	return 1
}

func f1(a int, b float32, c float64, d[] int, e[] float32, f[] float64) int  {
	fmt.Printf("funzione f1 int con argomenti\n")
	fmt.Printf("%d %f %f \n", a, b, c)
	fmt.Printf("%d %f %f \n", d[0], e[0], f[0])
	return 1
}

func f2() float32  {
	fmt.Printf("funzione f2 float senza argomenti \n")
	return 3.5
}

func f3(a int, b float32, c float64, d[] int, e[] float32, f[] float64) float32  {
	fmt.Printf("funzione f3 float con argomenti\n")
	fmt.Printf("%d %f %f \n", a, b, c)
	fmt.Printf("%d %f %f \n", d[0], e[0], f[0])
	return 1
}

func f4() float64  {
	fmt.Printf("funzione f4 double senza argomenti \n")
	return 1
}

func f5(a int, b float32, c float64, d[] int, e[] float32, f[] float64) float64  {
	fmt.Printf("funzione f4 double con argomenti\n")
	fmt.Printf("%d %f %f \n", a, b, c)
	fmt.Printf("%d %f %f \n", d[0], e[0], f[0])
	return 1
}

func f6()   {
	fmt.Printf("funzione f6 void senza argomenti \n")
	
}

func f7(a int, b float32, c float64, d[] int, e[] float32, f[] float64)   {
	fmt.Printf("funzione f6 void con argomenti\n")
	fmt.Printf("%d %f %f \n", a, b, c)
	fmt.Printf("%d %f %f \n", d[0], e[0], f[0])
}

func main()   {
	var a int = 1
	var b float32 = 2.2
	var c float64 = 3.3
	var d = make([]int, 1,1)
	var e = make([]float32, 1,1)
	var f = make([]float64, 1,1)
	d[0]=4
	e[0]=5.5
	f[0]=6.6
	fmt.Printf("Valore di ritorno di f0 %d \n", f0())
	fmt.Printf("Valore di ritorno di f1 %d \n", f1(a, b, c, d, e, f))
	fmt.Printf("Valore di ritorno di f2 %f \n", f2())
	fmt.Printf("Valore di ritorno di f3 %f \n", f3(a, b, c, d, e, f))
	fmt.Printf("Valore di ritorno di f4 %f \n", f4())
	fmt.Printf("Valore di ritorno di f5 %f \n", f5(a, b, c, d, e, f))
	f6()
	f7(a, b, c, d, e, f)
}

