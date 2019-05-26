#include<stdio.h>
#define SIZE 10
int main()
{	
	//define an array
	int arr[10];
	
	/*
	int a = 10;
	int b[a];
	Wrong will occur in some compiler
	[] is better for constant
	*/
	
	int c[SIZE];
	c[0] = 1;
	int i = 1;
	c[i] = 2;
	c[i+1] = 3;
	
	// Be careful for boundary
	// c[10] is wrong(0-9)
	
	//Only definition can init
	int a1[10] = {1,2,3,4,5,6,7,8,9,0};
	
	/*Err
	int a2[10];
	a2 = {1,2,3,4,5,6,7,8,9,0};
	*/

	//partial init 
	// other is 0
	int a3[10] = {1,2,3};
	int a4[10] = {0};

	//define and init together,the first [] can be empty
	int a5[] = {1,2,3};
	

}
