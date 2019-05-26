#include<stdio.h>
#include<stdlib.h> // system is in it 
int main()
{
	printf("before sys\n");
	// system is to call another appilcation
	system("ls");

	printf("after sys\n");
	return 0;
}
