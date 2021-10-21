package main 

import "fmt" 

func main()   {
	var i int
	for i=0; i<2; i=i+1 {
		fmt.Printf("i: %d\n", i)
	}
	for k := 2; k>0; k=k-1 {
		var k int = 7
		fmt.Printf("k: %d \n", k)
	}
}

