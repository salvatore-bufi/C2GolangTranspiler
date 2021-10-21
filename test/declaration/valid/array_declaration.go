package main 

import "fmt" 

func main()   {
	var a int = 1
	var v1 = make([]int, 3*4+3,3*4+3)
	var v2 = make([]float32, 3*6/2,3*6/2)
	var v3 = make([]float64, 9/3-a,9/3-a)
	v1[0]=1
	v2[0]=2.2
	v3[0]=3.3
	fmt.Printf("%d %f %f", v1[0], v2[0], v3[0])
}

