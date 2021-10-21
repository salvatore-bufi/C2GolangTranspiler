package main 

import "fmt" 

func print_array(v[] int, len int)   {
	fmt.Printf("\n \narray: \n")
	for i := 0; i<len; i=i+1 {
		fmt.Printf("%d \t ", v[i])
	}
	fmt.Printf("\n")
}

func bubblesort(v[] int, len int)   {
	var temp int
	for i := 0; i<len-1; i=i+1 {
		for k := 0; k<len-1-i; k=k+1 {
			if v[k]>v[k+1] {
				temp=v[k]
				v[k]=v[k+1]
				v[k+1]=temp
			}
		}
	}
}

func main()   {
	var len int
	var v = make([]int, 20,20)
	fmt.Printf("inserire la dimensione del vettore (max 20 elementi) \n")
	fmt.Scanf("%d", &len)
	if len>20||len<0 {
		fmt.Printf("dimensione non valida \n!")
	}else{
		for i := 0; i<len; i=i+1 {
			fmt.Printf("inserire l'elemento %d del vettore: \n", i)
			fmt.Scanf("%d", &v[i])
		}
		bubblesort(v, len)
		print_array(v, len)
	}
}

