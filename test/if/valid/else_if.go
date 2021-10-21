package main 

import "fmt" 

func main()   {
	var i int = 10
	if i==0 {
		fmt.Printf("i == 0 \n")
	}else{
		if i<0 {
			fmt.Printf("i < 0 \n")
		}else{
			fmt.Printf("i > 0 \n")
		}
	}
}

