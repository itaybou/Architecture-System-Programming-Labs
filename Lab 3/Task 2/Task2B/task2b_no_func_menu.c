#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

#define VIRUS_NAME_LENGTH 16
#define BUFFER_SIZE 10000

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

char * suspected_buffer;
link *signatures;

void PrintHex(char buffer[], size_t length);
void printVirus(struct virus *vir);
link *loadSignatures(char *filename);
long load_suspected(char *filename);
void detect_virus(char *buffer, unsigned int size);
void kill_virus(char *fileName, int signitureOffset, int signitureSize);

void list_print(link *virus_list);
link *list_append(link *virus_list, virus *data);
void list_free(link *virus_list);

int main(int argc, char **argv) {

  const size_t MAX_INPUT_SIZE = 30 * sizeof(char);
  const char *menu[] = {"Load signatures", "Print signatures",
                        "Detect viruses", "Fix file", "Quit", NULL};
  char *input = (char *)malloc(MAX_INPUT_SIZE), *suspected_filename = (char *)malloc(MAX_INPUT_SIZE);
  int i, choice, virSigSize, suspectedOffset;
  long suspected_size;

  suspected_buffer = NULL;
  signatures = NULL;

  while (true) {
    for (i = 0; menu[i] != NULL; i++)
      printf("%i) %s\n", i + 1, menu[i]);

    printf("Choice: ");
    fgets(input, MAX_INPUT_SIZE, stdin); //Get user numeric choice input
    sscanf(input, "%d", &choice);
    switch (choice) {
        case 1: // Load signatures
        printf("Filename: ");
        fgets(input, MAX_INPUT_SIZE, stdin);
        sscanf(input, "%s", input);
        if (signatures != NULL)
          list_free(signatures);
        signatures = loadSignatures(input);
        break;
      case 2: // Print signatures
        list_print(signatures);
        break;
      case 3: // Detect viruses
         printf("Filename: ");
         fgets(input, MAX_INPUT_SIZE, stdin);
         sscanf(input, "%s", suspected_filename);
         suspected_size = load_suspected(suspected_filename);
         if(suspected_size != -1) {
            detect_virus(suspected_buffer, (BUFFER_SIZE >= suspected_size) ? suspected_size : BUFFER_SIZE);
         }
        break;
      case 4: // Fix file
         printf("Enter starting byte SPACE and signature size (format: start size): ");
         fgets(input, MAX_INPUT_SIZE, stdin);
         sscanf(input, "%d %d", &suspectedOffset, &virSigSize);
         kill_virus(suspected_filename, suspectedOffset, virSigSize);
         break;
      case 5: //Exit
        list_free(signatures);
        free(input);
        free(suspected_filename);
        if(suspected_buffer != NULL)
            free(suspected_buffer);
        exit(0);
      default:
        printf("Invalid choice!\n");
        break;
    }
    printf("\n");
  }
  return 0;
}

link *loadSignatures(char *filename) {
  FILE *fp;
  link *virus_list = NULL;
  unsigned short virus_size;
  struct virus *vir;

  fp = fopen(filename, "rb");

  if (!fp)
    fprintf(stderr, "Unable to open %s file!\n\n", filename);
  else {
    while ((fread(&virus_size, 2 * sizeof(char), 1, fp)) != 0) { // while not zero items read
      vir = (struct virus *)malloc(virus_size); // Allocate memory as size of virus_size to enable allocating memory for flexible array (sizeof(struct virus) + sizeof(sig))
      fseek(fp, -2 * sizeof(char),SEEK_CUR); // Since 2 bytes representing size as been read, move 2 bytes backward in file to enable reading entire virus
      fread(vir, virus_size, 1, fp); // Read entire virus data unto allocated memory
      (*vir).SigSize -= VIRUS_NAME_LENGTH + 2 * sizeof(char); // Decrease the name length and the size from total size to get signature size
      virus_list = list_append(virus_list, vir);
    }
    fclose(fp);
  }
  return virus_list;
}

long load_suspected(char *filename)
{
   FILE *fp;
   long file_size = -1;

   fp = fopen(filename, "rb");
   if (!fp) {
     fprintf(stderr, "Unable to open %s file!\n\n", filename);
  }
   else {
      if(suspected_buffer != NULL)
         free(suspected_buffer);
      suspected_buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
      fread(suspected_buffer, BUFFER_SIZE, 1, fp);
      fseek(fp, 0 , SEEK_END);
      file_size = ftell(fp);  //Get suspected file size
      fseek(fp, 0 , SEEK_SET);
      fclose(fp);
   }
   return file_size;
}

void detect_virus(char *buffer, unsigned int size)
{
   link *curr = signatures;
   int i;

   while (curr != NULL) {
      for(i=0; i<size; i++) {
         if(memcmp(&buffer[i], curr->vir->sig, curr->vir->SigSize) == 0) //Compare bytes as size of virus signature from current buffer byte location with current iterated virus signature
         {
            printf("VIRUS FOUND!\n");
            printf("Starting byte: %d\n", i);
            printf("Virus name: %s\n", curr->vir->virusName);
            printf("Virus signature size: %d\n\n", curr->vir->SigSize);
         }
      }
      curr = curr->nextVirus;
   }
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize)
{
   FILE *fp;
   int i;
   const unsigned char NOP = 0x90;

   fp = fopen(fileName, "r+b"); // Open file for read/write binary
   if (!fp) {
     fprintf(stderr, "Unable to open %s file!\n\n", fileName);
   }
   else {
      for(i=0; i<signitureSize; i++) {
        fseek(fp, signitureOffset+i , SEEK_SET); // Go to signature offset in file
        fwrite(&NOP, 1, sizeof(NOP), fp); // Write signature size bytes to offset location
     }
     fclose(fp);
   }

}

void PrintHex(char buffer[], size_t length) {
  int i;
  for (i = 0; i < length; i++)
    printf("%02X%s", (unsigned char)buffer[i],
           ((i + 1) % 20 == 0 && i != length - 1) ? "\n" : " ");
}

void printVirus(struct virus *vir) {
  printf("Virus name: %s\n", (*vir).virusName);
  printf("Virus size: %d\n", (*vir).SigSize);
  printf("signature:\n");
  PrintHex((*vir).sig, (*vir).SigSize);
  printf("\n\n");
}

void list_print(link *virus_list) {
  link *curr = virus_list;

  while (curr != NULL) {
    printVirus(curr->vir);
    curr = curr->nextVirus;
  }
}

link *list_append(link *virus_list, virus *data) {
  // Append at the end of list
  link *curr = virus_list;
  if (curr == NULL) {
    curr = (link *)malloc(sizeof(link));
    curr->vir = data;
    curr->nextVirus = NULL;
    return curr;
  }

  while (curr->nextVirus != NULL)
    curr = curr->nextVirus;

  curr->nextVirus = (link *)malloc(sizeof(link));
  curr->nextVirus->vir = data;
  curr->nextVirus->nextVirus = NULL;

  // Append at start of list
  /*link *newLink = (link*)malloc(sizeof(link));
  newLink->vir = data;
  newLink->nextVirus = virus_list;
  virus_list = newLink;*/

  return virus_list;
}

void list_free(link *virus_list) {
  link *curr = virus_list;
  link *temp;

  while (curr != NULL) {
    free(curr->vir);
    temp = curr;
    curr = curr->nextVirus;
    free(temp);
  }
}
