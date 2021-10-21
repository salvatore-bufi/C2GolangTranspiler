package main 

import "fmt" 

var b int = 2
func f(c int)   {
	fmt.Printf("FUNCTION: local var:%d  \t global var: %d \n", c, b)
	for b := 0; b<2; b=b+1 {
		fmt.Printf("For scope %d \t func variable %d \n", b, c)
	}
}

func main()   {
	var a int = 1
	fmt.Printf("MAIN: local var:%d  \t global var: %d \n", a, b)
	f(3)
}

