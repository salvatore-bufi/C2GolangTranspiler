package main 

import "fmt" 

func main()   {
	for i := 0; i<2; i=i+1 {
		fmt.Printf("%d \n", i)
	}
	var i int
	for i=0; i<2; i=i+1 {
		fmt.Printf("%d \n", i)
	}
}

