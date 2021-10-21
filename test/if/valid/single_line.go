package main 

import "fmt" 

func main()   {
	if 1>0 {
		fmt.Printf("success \n")
	}else{
		fmt.Printf("fail \n")
	}
	if 1<0 {
		fmt.Printf("fail \n")
	}else{
		fmt.Printf("success \n")
	}
	if 1>0 {
		fmt.Printf("success \n")
	}
}

