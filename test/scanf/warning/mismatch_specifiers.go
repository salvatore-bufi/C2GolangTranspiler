package main 

import "fmt" 

func main()   {
	var a int
	var b float32
	var d float64
	var e float64
	fmt.Scanf("%d %f %f %f", a, b, d, e)
	fmt.Scanf("%f %d %d", &a, &b, &d)
}

