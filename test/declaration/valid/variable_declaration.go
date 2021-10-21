package main 

import "fmt" 

func main()   {
	var a int = 1/2
	var a1 int = a+2
	var b float32 = 1+3.4
	var b1 float32 = b
	var c float64 = 7+2
	var c1 float64 = c+9.7
	fmt.Printf("%d %f %f \n", a, b, c)
	fmt.Printf("%d %f %f \n", a1, b1, c1)
}

