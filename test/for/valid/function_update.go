package main 

import "fmt" 

var i int
func f()   {
	i=i+1
}

func main()   {
	for i=0; i<2; f() {
		fmt.Printf("i: %d \n", i)
	}
}

