#include<stdio.h>

int main()
{
	int a[10];
	//%p address
	printf("a = %p, &a[0] = %p\n", a, &a[0]);

	//sizeof  unsigned long
	printf("sizeof(a) = %lu\n", sizeof(a));

	//Get num
	printf("number:%lu\n",sizeof(a)/sizeof(a[0]));
	return 0;

}
