#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (int argc, char** argv){ 
	int iarray[] = {1,2,3};
	char carray[] = {'a','b','c'};
	int* iarrayPtr;
	char* carrayPtr;
	int *p;
	int i;

	//Assign pointers to the address of first cell in each array
	iarrayPtr = &iarray[0]; 
	carrayPtr = &carray[0];
	
	//Print the value that each pointer points to incremented
	printf("Integer array:\n");
	//Size of array in bytes divided by size of type in bytes
	for(i=0; i<(sizeof(iarray)/sizeof(int)); i++)
		printf("%i, ", *(iarrayPtr+i));
	
	printf("\nChar array:\n");
	for(i=0; i<(sizeof(carray)/sizeof(char)); i++)
	printf("%c, ", *(carrayPtr+i));
	
	//Suppose to point to random data if not initialized to null
	//The value will change in each run because it is allocated in runtime
	printf("\nValue of uninitalized pointer p is: \n"); 
	printf("%s", *p);

}
