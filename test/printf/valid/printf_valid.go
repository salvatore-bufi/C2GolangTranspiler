package main 

import "fmt" 

func main()   {
	var a int = 1
	var b float32 = 2.2
	var c float64 = 3.3
	var v1 = make([]int, 1,1)
	var v2 = make([]float32, 1,1)
	var v3 = make([]float64, 1,1)
	v1[0]=11
	v2[0]=22.2
	v3[0]=33.3
	fmt.Printf("printf int: %d /t %d \n", a, a)
	fmt.Printf("printf float: %f \t %F \t %e \t %E \t %g \t %G\n", b, b, b, b, b, b)
	fmt.Printf("printf double: %f \t %F \t %e \t %E \t %g \t %G\n", c, c, c, c, c, c)
	fmt.Printf("printf vettori: int %d, \t float %f, \t double %f \n", v1[0], v2[0], v3[0])
	fmt.Printf("printf percentuale: %%f \n")
}

