package main 

import "fmt" 

func main()   {
	var a int
	var b float32
	var c float64
	var v1 = make([]int, 1,1)
	var v2 = make([]float32, 1,1)
	var v3 = make([]float64, 1,1)
	fmt.Scanf("%d", &a)
	fmt.Scanf("%f", &b)
	fmt.Scanf("%f", &c)
	fmt.Scanf("%d", &v1[0])
	fmt.Scanf("%f", &v2[0])
	fmt.Scanf("%f", &v3[0])
	fmt.Printf("printf int: %d\n", a)
	fmt.Printf("printf float: %f\n", b)
	fmt.Printf("printf double: %f\n", c)
	fmt.Printf("printf vettori: int %d, \t float %f, \t double %f \n", v1[0], v2[0], v3[0])
}

