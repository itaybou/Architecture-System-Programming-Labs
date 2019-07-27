#include <stdio.h>
#include <stdlib.h>

void PrintHex(unsigned char *buffer, size_t length);

int main(int argc, char **argv) {
    FILE * input;
	unsigned long fileSize;
	unsigned char * buffer;
	
	if ((input = fopen(argv[1],"r")) == NULL) {
		printf("Invalid file specified.");
		exit(1);
    }
    
    fseek(input, 0, SEEK_END);
	fileSize = ftell(input);
	fseek(input, 0 , SEEK_SET);
	buffer = (unsigned char*)calloc(fileSize, sizeof(unsigned char));
	
	fread(buffer, sizeof(unsigned char), fileSize, input);
	
	PrintHex(buffer, fileSize);
	
	fclose(input);
	free(buffer);
	
    return 0;
}

void PrintHex(unsigned char *buffer, size_t length)
{
	int i;
	for(i=0; i<length; i++)
	{
		printf("%02X ", *(buffer+i));
	}
}
