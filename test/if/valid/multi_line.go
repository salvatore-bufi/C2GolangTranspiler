package main 

import "fmt" 

func main()   {
	if 1<2 {
		fmt.Printf("success \n")
		var a int = 0
		fmt.Printf("%d \n", a)
	}else{
		fmt.Printf("fail \n")
	}
	if 1>0 {
		fmt.Printf("success \n")
		var a int = 1
		fmt.Printf("%d \n", a)
	}else{
		fmt.Printf("fail \n")
	}
	if 1==1 {
		fmt.Printf("success \n")
		var a int = 2
		fmt.Printf("%d \n", a)
	}else{
		fmt.Printf("fail \n")
	}
	if 1!=2 {
		fmt.Printf("success \n")
		var a int = 3
		fmt.Printf("%d \n", a)
	}else{
		fmt.Printf("fail \n")
	}
	if 1!=2&&1<2 {
		fmt.Printf("success \n")
		var a int = 4
		fmt.Printf("%d \n", a)
	}else{
		fmt.Printf("fail \n")
	}
	if 1!=1||1<2 {
		fmt.Printf("success \n")
		var a int = 5
		fmt.Printf("%d \n", a)
	}else{
		fmt.Printf("fail \n")
	}
	if !(1==1) {
		fmt.Printf("fail \n")
	}else{
		fmt.Printf("success \n")
		var a int = 6
		fmt.Printf("%d \n", a)
	}
}

