#include "my_function.h"
#include <stdio.h>

int my_function(char a)
{
	(void) &a;

	printf("*****************************\n");
	printf("PROTECTING THIS PIECE OF CODE\n");
	printf("*****************************\n");
	printf("\n");

	int h = 12;
	for (int i = 1; i < h; i++)
	{
		for (int j = 0; j < (h-i); j++)
			printf(" ");
		
		for (int j = 0; j < i; j++) 
			printf("*");
		
		for (int j = 0; j < (h-i); j++)
			printf(" ");
		
		printf("\n");
	}
	return 0;
}

void dummy_function(void) 
{ return; }