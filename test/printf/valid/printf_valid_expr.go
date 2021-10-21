package main 

import "fmt" 

func f(a int) int  {
	return a
}

func main()   {
	var a int = 1
	var b int = 2
	fmt.Printf("%d \n", a+b)
	fmt.Printf("%d \n", a*b)
	fmt.Printf("%d \n", a-b)
	fmt.Printf("%d \n", b/a)
	fmt.Printf("%d \n", f(a))
}

