#include<stdio.h>

int max(int a, int b, int c);

int main()
{
	int a=4, b=5, c=6;
	printf("Max : %d\n", max(a,b,c));
	return 0;
}

int max(int a,int b,int c)
{
	int max = a;
	if(a < b)
	{
		max = b;
	}
	if(max < c)
	{
		max = c;
	}
	return max;
}
