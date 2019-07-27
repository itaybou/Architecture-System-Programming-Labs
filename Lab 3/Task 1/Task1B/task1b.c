#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0

#define VIRUS_NAME_LENGTH 16

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char sig[];
} virus;

typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};

void PrintHex(char buffer[], size_t length);
void printVirus(struct virus* vir);
link* loadSignatures(char *filename);

void list_print(link *virus_list);
link* list_append(link* virus_list, virus* data);
void list_free(link *virus_list);

int main(int argc, char **argv) {
	
	const char* menu[] = { "Load signatures", "Print signatures", "Quit", NULL };
	link *signatures = NULL;
	char *input = (char *)malloc(30*sizeof(char));
	int i, choice;
	
	while(true)
	{
		for(i = 0; menu[i] != NULL; i++)
			printf("%i) %s\n",i+1 , menu[i]);
		
		fgets(input, 30*sizeof(char), stdin);
		sscanf(input, "%d", &choice);
		switch(choice) {
			case 1:
				fgets(input, 30*sizeof(char), stdin);
				sscanf(input, "%s", input);
				if(signatures != NULL)
					list_free(signatures);
				signatures = loadSignatures(input);
				break;
			case 2:
				list_print(signatures);
				break;
			case 3:
				list_free(signatures);
				free(input);
				exit(0);
			default:
				printf("Invalid choice!\n");
				break;
		}
	}
    return 0;
}

link* loadSignatures(char *filename)
{
	FILE * fp;
	link* virus_list = NULL;
	unsigned short virus_size;
	struct virus *vir;
	
	fp = fopen(filename, "rb");
	
	if (!fp)
		fprintf(stderr, "Unable to open %s file!\n\n", filename);
	else{
		while((fread(&virus_size, 2*sizeof(char), 1, fp)) != 0)
		{
			vir = (struct virus*)malloc(virus_size); 				// Allocate memory as size of virus_size to enable allocating memory for flexible array (sizeof(struct virus) + sizeof(sig))
			fseek(fp, -2*sizeof(char), SEEK_CUR); 					// Since 2 bytes representing size as been read, move 2 bytes backward in file to enable reading entire virus
			fread(vir, virus_size, 1, fp); 							// Read entire virus data unto allocated memory
			(*vir).SigSize -= VIRUS_NAME_LENGTH + 2*sizeof(char); 	// Decrease the name length and the size from total size to get signature size
			virus_list = list_append(virus_list, vir);
		}
		fclose(fp);
	}
	return virus_list;
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

void list_print(link *virus_list)
{
	link *curr = virus_list;
	
	while(curr != NULL)
	{
		printVirus(curr->vir);
		curr = curr->nextVirus;
	}
}

link* list_append(link* virus_list, virus* data)
{
	//Append at the end of list
	link *curr = virus_list;
	if(curr == NULL) {
		curr = (link*)malloc(sizeof(link));
		curr->vir = data;
		curr->nextVirus = NULL;
		return curr;
	}
	
	while(curr->nextVirus != NULL)
		curr = curr->nextVirus;
	
	curr->nextVirus = (link*)malloc(sizeof(link));
	curr->nextVirus->vir = data;
	curr->nextVirus->nextVirus = NULL;

	
	//Append at start of list
	/*link *newLink = (link*)malloc(sizeof(link));
	newLink->vir = data;
	newLink->nextVirus = virus_list;
	virus_list = newLink;*/
	
	return virus_list;	
}

void list_free(link *virus_list)
{
	 link* curr = virus_list;
	 link* temp;

    while (curr != NULL) {
		free(curr->vir);
		temp = curr;
		curr = curr->nextVirus;
		free(temp);
    }
}
