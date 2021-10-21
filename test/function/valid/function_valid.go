package main 

import "fmt" 

func f(a int)   {
	fmt.Printf("%d \n", a)
}

func f1() int  {
	return 1
}

func f2() int  {
	var a int = f1()
	return a
}

func main()   {
	var a int = 1
	var b int = 1
	f(a+b)
	f(a-b)
	f(a*b)
	f(a/b)
	f(f2())
}

