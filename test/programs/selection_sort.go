package main 

import "fmt" 

func print_vay(v[] int, len int)   {
	fmt.Printf("\n \narray: \n")
	for i := 0; i<len; i=i+1 {
		fmt.Printf("%d \t ", v[i])
	}
	fmt.Printf("\n")
}

func selectionsort(v[] int, len int)   {
	var min int = v[0]
	var i int = 0
	var min_index int = 0
	var j int = 0
	for i=0; i<len-1; i=i+1 {
		min=v[i]
		min_index=i
		for j=i+1; j<len; j=j+1 {
			if v[j]<min {
				min=v[j]
				min_index=j
			}
		}
		v[min_index]=v[i]
		v[i]=min
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
		selectionsort(v, len)
		print_vay(v, len)
	}
}

