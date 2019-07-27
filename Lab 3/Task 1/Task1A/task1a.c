#include <stdio.h>
#include <stdlib.h>

#define VIRUS_NAME_LENGTH 16

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char sig[];
} virus;

void PrintHex(char buffer[], size_t length);
void printVirus(struct virus* vir);

int main(int argc, char **argv) {
	FILE * fp;
	const char* file_name ="signatures";
	unsigned short virus_size;
	struct virus *vir;
	
	fp = fopen(file_name, "rb");
	
	if (!fp)
	{
		fprintf(stderr, "Unable to open %s file!", file_name);
		exit(1);
	}
	
	while((fread(&virus_size, 2*sizeof(char), 1, fp)) != 0)
	{
		vir = (struct virus*)malloc(virus_size); 				// Allocate memory as size of virus_size to enable allocating memory for flexible array (sizeof(struct virus) + sizeof(sig))
		fseek(fp, -2*sizeof(char), SEEK_CUR); 					// Since 2 bytes representing size as been read, move 2 bytes backward in file to enable reading entire virus
		fread(vir, virus_size, 1, fp); 							// Read entire virus data unto allocated memory
		(*vir).SigSize -= VIRUS_NAME_LENGTH + 2*sizeof(char); 	// Decrease the name length and the size from total size to get signature size
		printVirus(vir); 										// Print virus details
		free(vir);												// Free allocated memory for virus
	}
	
	fclose(fp);
    return 0;
}

void PrintHex(char buffer[], size_t length)
{
	int i;
	for(i=0; i<length; i++)
		printf("%02X%s", (unsigned char)buffer[i], ((i+1) % 20 == 0 && i != length-1) ? "\n" : " ");
}

void printVirus(struct virus* vir)
{
	printf("Virus name: %s\n", (*vir).virusName);
	printf("Virus size: %d\n", (*vir).SigSize);
	printf("signature:\n");
	PrintHex((*vir).sig, (*vir).SigSize);
	printf("\n\n");
}
