package main 

import "fmt" 

func main()   {
	var c int
	fmt.Printf("inserire caso da provare [1-4]: \n")
	fmt.Scanf("%d", &c)
	if c==1 {
		for i := 0; ; i=i+1 {
			fmt.Printf("NO_COND loop \n")
		}
	}else{
		if c==2 {
			for i := 0; ;  {
				i=i+1
				fmt.Printf("NO_COND & NO_UPDATE loop \n")
			}
		}else{
			if c==3 {
				var i int = 0
				for ; ; i=i+1 {
					fmt.Printf("NO_INIT & NO_COND loop \n")
				}
			}else{
				for ; ;  {
					fmt.Printf("NO_INIT & NO_COND & NO_UPDATE loop \n")
				}
			}
		}
	}
}

