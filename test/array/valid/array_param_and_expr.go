package main 

import "fmt" 

func sum_array(v1[] int, v2[] int, len int)   {
	for i := 0; i<len; i=i+1 {
		fmt.Printf("%d \n", v1[i]+v2[i])
	}
}

func diff_array(v1[] int, v2[] int, len int)   {
	for i := 0; i<len; i=i+1 {
		fmt.Printf("%d \n", v1[i]-v2[i])
	}
}

func mul_array(v1[] int, v2[] int, len int)   {
	for i := 0; i<len; i=i+1 {
		fmt.Printf("%d \n", v1[i]*v2[i])
	}
}

func div_array(v1[] int, v2[] int, len int)   {
	for i := 0; i<len; i=i+1 {
		fmt.Printf("%d \n", v1[i]/v2[i])
	}
}

func main()   {
	var a = make([]int, 2,2)
	var b = make([]int, 2,2)
	a[0]=8
	a[1]=2
	b[0]=2
	b[1]=8
	sum_array(a, b, 2)
	diff_array(a, b, 2)
	mul_array(a, b, 2)
	div_array(a, b, 2)
}

