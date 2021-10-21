package main 

import "fmt" 

func main()   {
	if 1<2 {
		fmt.Printf("1st if \n")
		if 2>0 {
			fmt.Printf("2nd if \n")
			if 3>2 {
				fmt.Printf("3rd if \n")
			}
		}
	}
}

