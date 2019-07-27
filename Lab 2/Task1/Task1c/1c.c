#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (int argc, char** argv){ 
	int iarray[3];
	float farray[3];
	double darray[3];
	char carray[3];

	
	printf("The hex value of iarray: %p\n", iarray);
	printf("The hex value of iarray+1: %p EXPECTED + %d\n\n", iarray+1, sizeof(int)); //int array expected address + 4;
	printf("The hex value of farray: %p\n", farray);
	printf("The hex value of farray+1: %p EXPECTED + %d\n\n", farray+1, sizeof(float)); //float array expected address + 4;
	printf("The hex value of darray: %p\n", darray);
	printf("The hex value of darray+1: %p EXPECTED + %d\n\n", darray+1, sizeof(double)); //double array expected address + 8;
	printf("The hex value of carray: %p\n", carray);
	printf("The hex value of carray+1: %p EXPECTED + %d\n\n", carray+1, sizeof(char)); //char array expected address + 1;
}
