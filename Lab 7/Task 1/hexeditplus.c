#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KB(i) ((i)*1<<10)
#define INT sizeof(int)
#define SHORT sizeof(short)
#define BYTE sizeof(char)

#define MAX_FILE_SIZE KB(10)

typedef char bool;
#define true '1'
#define false '0'

typedef struct state {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
} state;

struct fun_desc{
 char *name;
 void (*fun)(state*);
};

int print_menu();
void assign_defaults(state * st);
void quit(state * st);
void toggle(state * st);
void set_filename(state * st);
void set_unitsize(state * st);
void mem_load(state* st);
void mem_display(state* st);
void saveinfile(state* st);
void file_modify(state * st);

char* unit_to_format(int unit);
void print_mem(state * st, unsigned char * addr, int count);

int unit_sizes[] = { BYTE, SHORT, INT,  };

struct fun_desc menu[]={{"Toggle Debug Mode",&toggle}, {"Set File Name", &set_filename}, {"Set Unit Size", &set_unitsize}, {"Load Into Memory", &mem_load},
                        {"Memory Display", &mem_display}, {"Save Into File", &saveinfile}, {"File Modify", &file_modify}, {"Quit", &quit}, {NULL,NULL}};
state * s;

int main(int argc, char **argv)
{
   int option;
   s = (state *)malloc(sizeof(struct state));
   size_t bounds = sizeof(menu)/sizeof(struct fun_desc)-1;
   assign_defaults(s);
    while(true)
    {
       if(s->debug_mode == true)
       {
           printf("Unit size : %d\n", s->unit_size);
           printf("File Name : %s\n", s->file_name);
           printf("Mem count : %zu\n", s->mem_count);
       }
       option = print_menu();
       if(option >= bounds || option < 0){
           printf("Not within bounds\n");
       }
       else
       {
           printf("Within bounds.\n");
           (menu[option].fun)(s);
       }
    }

    return 0;
}

void assign_defaults(state * st)
{
   st->debug_mode = false;
   st->unit_size = BYTE;
   st->mem_count = 0;
}

int print_menu()
{
   int i = 0, choice;
   char * in = (char *)malloc(5);
   printf("\nChoose action:\n");
   while(menu[i].name != NULL)
   {
       printf("%d- %s\n", i, menu[i].name);
       ++i;
   }
   printf("Option: ");
   fgets(in, 10, stdin);
   sscanf(in,"%d", &choice);
   free(in);
   return choice;
}

void quit(state* st)
{
    if(st->debug_mode == true)
        printf("quitting\n");
    exit(0);
}

void toggle(state* st)
{
   printf(st->debug_mode == true ? "Debug flag now off\n" : "Debug flag now on\n");
   st->debug_mode = s->debug_mode == true ? false : true;
}

void set_filename(state * st)
{
   char* input = (char*)malloc(100);
   printf("Please enter file name: ");
   input = fgets(input, 100, stdin);
   sscanf(input,"%s", st->file_name);
	if(st->debug_mode == true)
        printf("Debug: file name set to '%s'\n", st->file_name);
   free(input);
}

void set_unitsize(state * st)
{
   char* input=(char*)malloc(100);
   int unitsize = 0;
   printf("Please enter unit size: ");
   input = fgets(input,100,stdin);
	sscanf(input, "%d", &unitsize);
   if(unitsize != BYTE && unitsize != SHORT && unitsize != INT)
        printf("not a valid unit size.\n");
    else
    {
        st->unit_size = unitsize;
        if(st->debug_mode==true)
            printf("Debug: set size to %d\n", unitsize);
    }
    free(input);

}

void mem_load(state * st)
{
    FILE* fp;
    char* input=(char*)malloc(100);
    int length,location;
    if(st->file_name[0] == 0){
        printf("Error opening file.\n");
        free(input);
        return;
    }

    fp = fopen(s->file_name, "r");
    if(!fp)
    {
        printf("Error opening file.\n");
        free(input);
        return;
    }
    else
    {
        printf("Please enter <location> <length>\n");
        fgets(input, 100, stdin);
        sscanf(input,"%x %d", &location, &length);
        if(st->debug_mode == true)
        {
                printf("File name: %s\n", st->file_name);
                printf("Location: %x\n", location);
                printf("Length: %d\n", length);
        }
        fseek(fp, location, SEEK_SET);
        int success = fread(s->mem_buf, st->unit_size, length, fp);
        if(success > 0)
        {
            printf("Loaded %d units into memory.\n", length);
            st->mem_count = length * s->unit_size;
        }
        else printf("Error reading bytes from file.\n");
        fclose(fp);
    }
    free(input);
}

char* unit_to_format(int unit)
{
    static char* formats[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};
    return formats[unit-1];
}

void mem_display(state* st)
{
    char* input = (char*)malloc(100);
    unsigned char * address;
    unsigned char * temp;
    int u;
    printf("Please enter <address> <length>\n");
    fgets(input,100,stdin);
    sscanf(input,"%p %d",&address, &u);
     if(address==0)
     {
         printf("Decimal\t\tHexadecimal\n");
         printf("========================\n");
         temp = st->mem_buf;
         print_mem(st, temp, u);
     }
     else print_mem(st, address, u);
     free(input);
}

void print_mem(state * st, unsigned char * addr, int count)
{
   unsigned char * end = addr + st->unit_size * count;
   while (addr < end)
   {
      char var1 = *((char *)(addr));
      short var2 = *((short *)(addr));
      int var3 = *((int *)(addr));
      switch (st->unit_size)
      {
           case BYTE:
              printf("%d\t\t", var1);
              printf(unit_to_format(st->unit_size), var1);
              break;
           case SHORT:
              printf("%d\t\t", var2);
              printf(unit_to_format(st->unit_size), var2);
              break;
           case INT:
              printf("%d\t\t", var3);
              printf(unit_to_format(st->unit_size), var3);
              break;
           default:
               break;
      }
      addr += st->unit_size;
    }
}

void saveinfile(state* state)
{
    FILE* fp;
    char* input=(char*)malloc(100);
    unsigned char * slocation;
    int tlocation, length, filesize;
    if(s->file_name[0]==0){
       printf("Error opening file.\n");
       free(input);
        return;
    }
    fp=fopen(s->file_name,"r+");
    if(!fp)
    {
        printf("Error opening file.\n");
        free(input);
        return;
    }
    printf("Please enter <source-address> <target-location> <length>\n");
    fgets(input,100,stdin);
    sscanf(input,"%p %x %d", &slocation, &tlocation, &length);
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    if(filesize < tlocation)
        printf("ERROR : target is greater then file size");
    else
    {
        fseek(fp, 0, SEEK_SET);
        fseek(fp, tlocation, SEEK_SET);
        if(slocation == 0)
            fwrite(s->mem_buf, s->unit_size, length, fp);
        else fwrite(slocation, s->unit_size, length, fp);
    }
    fclose(fp);
    free(input);
}

void file_modify(state * state)
{
    FILE* fp;
    char* input = (char*)malloc(100);
    int location, val, filesize;
    if(s->file_name[0] == 0)
    {
      printf("Error opening file.\n");
      free(input);
      return;
    }

    fp = fopen(s->file_name,"r+");
    if(!fp)
    {
      printf("Error opening file.\n");
      free(input);
      return;
    }
    printf("Please enter <location> <val>\n");
    fgets(input,100,stdin);
    sscanf(input,"%x %x", &location, &val);
    if(s->debug_mode == true)
        printf("Location: %x Value: %x \n",location,val);
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    if(filesize<location)
        printf("ERROR : target is greater then file size");
    else
    {
        fseek(fp, 0, SEEK_SET);
        fseek(fp, location, SEEK_SET);
        fwrite(&val, 1, s->unit_size, fp);
    }
    fclose(fp);
    free(input);
}

/*Check for memory
====================
char num[] = "ab";
unsigned char *ptr = num;
printf("Address of num: %p\n", ptr);
printf("The value at address = `%c`, decimal %d, hex 0x%02x\n", *ptr, *ptr, *ptr);
 */
