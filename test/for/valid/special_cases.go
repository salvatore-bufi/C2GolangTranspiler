package main 

import "fmt" 

func main()   {
	var i int = 0
	for ; i<2; i=i+1 {
		fmt.Printf("NO_INIT i: %d \n", i)
	}
	for i=0; i<2;  {
		fmt.Printf("NO_UPDATE i: %d \n", i)
		i=i+1
	}
	i=0
	for ; i<2;  {
		fmt.Printf("NO_INIT && NO_UPDATE i: %d \n", i)
		i=i+1
	}
}

