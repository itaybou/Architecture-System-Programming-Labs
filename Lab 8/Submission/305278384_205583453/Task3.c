#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

typedef char bool;
#define true 1
#define false 0

#define isELF 127
#define FILE_CLOSED -1

#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02

struct fun_desc
{
 char *name;
 void (*fun)(void);
};


int print_menu();
void quit();
void toggle();
void Examine_ELF();
void print_sections();
void print_symbols();
void relocate_tables_row();
void relocate_tables_semantic();

struct fun_desc menu[]={{"Toggle Debug Mode",&toggle}, {"Examine ELF File", &Examine_ELF},{"Print Section Names",&print_sections},{"Print Symbols",&print_symbols},{"Relocation Tables-raw format",&relocate_tables_row},{"Relocation Tables-semantic format",&relocate_tables_semantic}, {"Quit", &quit}, {NULL,NULL}};

int Currentfd=FILE_CLOSED;
void *map_start; /* will point to the start of the memory mapped file */
struct stat fd_stat; /* this is needed to  the size of the file */
Elf32_Ehdr *header; /* this will point to the header structure */
int num_of_section_headers;
int debug_mode=false;
char file_name[100];
char* string_table;
int numofsections;

int main(int argc, char **argv)
{
   int option;
   size_t bounds = sizeof(menu)/sizeof(struct fun_desc)-1;
    while(true)
    {
       option = print_menu();
       if(option >= bounds || option < 0){
           printf("Not within bounds\n");
       }
       else
       {
           printf("Within bounds.\n");
           (menu[option].fun)();
       }
    }
    return 0;
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

void quit()
{
    if(debug_mode == true)
        printf("Quitting program...");
    exit(0);
}

void toggle()
{
   printf(debug_mode == true ? "Debug flag now off\n" : "Debug flag now on\n");
   debug_mode = debug_mode == true ? false : true;
}

void Examine_ELF()
{
   char* input = (char *) malloc(100);
   printf("Please enter file name: ");
   input = fgets(input, 100, stdin);
   sscanf(input,"%s", file_name);
   Elf32_Shdr* sectionpointer;
   Elf32_Shdr* sectionheader;
    if(Currentfd!=FILE_CLOSED){
        munmap(map_start,fd_stat.st_size);
        close(Currentfd);
    }
    if((Currentfd = open(file_name, O_RDWR)) < 0 ) {
      perror("Error opening file");
      exit(-1);
   }

   if( fstat(Currentfd, &fd_stat) != 0 ) {
      perror("stat operation failed");
      exit(-1);
   }

   if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, Currentfd, 0)) == MAP_FAILED ) {
      perror("mmap operation failed");
      exit(-4);
   }

   header = (Elf32_Ehdr *) map_start;

   printf("%s","The Magic numbers(in ASCII) : ");
   printf("%d",header->e_ident[1]);
   printf("%d",header->e_ident[2]);
   printf("%d\n",header->e_ident[3]);

   if((int)(header->e_ident[0])==isELF && (header->e_ident[1])=='E' && (header->e_ident[2])=='L'&& (header->e_ident[3])=='F')
   {
       if(header->e_ident[EI_DATA]==ELFDATANONE)
           printf("invalid data encoding\n");
       if(header->e_ident[EI_DATA]==ELFDATA2LSB)
           printf("data encoding: 2's complement, little endian\n");
       if(header->e_ident[EI_DATA]==ELFDATANONE)
           printf("data encoding: 2's complement, big endian\n");
       printf("Entry point : %x\n",header->e_entry);
       printf("Section header string table index: %d\n", header->e_shoff);
       printf("Number of section headers: %d\n", header->e_shnum);
       printf("Size of section headers: %d\n", header->e_shentsize);
       printf("Start of program headers: %d\n", header->e_phoff);
       printf("The number of program header entries: %d\n", header->e_phnum);
       printf("The size of each program header entry: %d\n", header->e_phentsize);
       sectionheader=(Elf32_Shdr*)(map_start+header->e_shoff);
       sectionpointer=(Elf32_Shdr*) (map_start + header->e_shoff +header->e_shstrndx * header->e_shentsize);
       string_table=(char*) map_start + sectionpointer->sh_offset + sectionheader->sh_name;
       numofsections=header->e_shnum;
   }
   else{
       fprintf(stderr,"ERROR : not ELF file");
        if(Currentfd!=FILE_CLOSED){
            munmap(map_start,fd_stat.st_size);
            close(Currentfd);
            Currentfd=FILE_CLOSED;
        }
   }
}

char* sectiontypes(int type)
{
    return type==0 ? "NULL":
    type==1 ? "PROGBITS" :
    type==2 ? "SYMTAB" :
    type==3 ? "STRTAB" :
    type==4 ? "RELA" :
    type==5 ? "HASH" :
    type==6 ? "DYNAMIC" :
    type==7 ? "NOTE" :
    type==8 ? "NOBITS" :
    type==9 ? "REL" :
    type==10 ? "SHLIB" :
    type==11 ? "DYNSYM" :
    type==14 ? "INIT_ARRAY" :
    type==15 ? "FINI_ARRAY" :
    type==16 ? "PREINIT_ARRAY" :
    type==17 ? "GROUP" :
    type==18 ? "SYMTAB_SHNDX" :
    type==19 ? "NUM" :
    type==SHT_LOOS ? "LOOS" :
    type==SHT_GNU_LIBLIST ? "GNU_LIBLIST" :
    type==SHT_CHECKSUM ? "CHECKSUM" :
    type==SHT_LOSUNW ? "LOSUNW" :
    type==SHT_SUNW_move ? "SUNW_move":
    type==SHT_SUNW_COMDAT ? "SUNW_COMDAT" :
    type==SHT_SUNW_syminfo ? "SUNW_syminfo" :
    type==SHT_GNU_verdef ? "GNU_verdef" :
    type==SHT_GNU_verneed ? "GNU_verneed" :
    type==SHT_GNU_versym ? "GNU_versym" :
    type==SHT_HISUNW ? "HISUNW" :
    type==SHT_HIOS ? "HIOS" :
    type==SHT_LOPROC ? "LOPROC" :
    type==SHT_HIPROC ? "HIPROC" :
    type==SHT_LOUSER ? "LOUSER" :
    type==SHT_HIUSER ? "HIUSER" : "NULL";
}

void print_sections()
{
    Elf32_Shdr* section;
    int i;
    if(Currentfd!=FILE_CLOSED)
    {
        section=(Elf32_Shdr*)(map_start+header->e_shoff);
        for(i = 0; i < numofsections; i++){
            printf("[%d]\t %-20s %-10x\t %-10x\t %-10x\t %-20s\n", i, string_table + section->sh_name, section->sh_addr,
                section->sh_offset, section->sh_size, sectiontypes(section->sh_type));
            section++;
        }
    } else fprintf(stderr, "No file currently open!");
}

void print_symbols()
{
    Elf32_Shdr* section;
    Elf32_Shdr* sectionheader;
    Elf32_Sym* symboltable;
    char*  symbol_string_table;
    char* sectioname;
    int i,j;
    if(Currentfd!=FILE_CLOSED)
    {
        sectionheader=(Elf32_Shdr*)(map_start+header->e_shoff);
        section=(Elf32_Shdr*)(map_start+header->e_shoff);
        for( i = 0; i < numofsections; i++){
            if(section->sh_type==SHT_SYMTAB || section->sh_type==SHT_DYNSYM){
                symboltable=(Elf32_Sym *)(map_start + section->sh_offset);
                if(debug_mode==true)
                     printf("symbol table %s contains %d entries :\n",string_table+section->sh_name,section->sh_size/section->sh_entsize);
                printf("Num:\t Value\t Section index\t Section name\t Symbol name\n");
                for(j=0; j<section->sh_size/section->sh_entsize; j++){
                    if(symboltable->st_shndx == SHN_ABS)
                        sectioname="";
                    else
                        sectioname = string_table + sectionheader[symboltable->st_shndx].sh_name;
                symbol_string_table = map_start + sectionheader[section->sh_link].sh_offset; /*symbol string table section offset */
                 printf("[%d]\t %08x\t %02d\t %s\t\t %-20s", j, symboltable->st_value, symboltable->st_shndx,
                   sectioname, symbol_string_table+symboltable->st_name );
                 printf("\n");
                symboltable++;
                }
            }
            section++;
        }
    } else fprintf(stderr, "No file currently open!");
}

void relocate_tables_row()
{
    Elf32_Shdr* section;
    Elf32_Rel* relocate;
    int i,j;
    if(Currentfd!=FILE_CLOSED)
    {
        section=(Elf32_Shdr*)(map_start+header->e_shoff);
        for( i = 0; i < numofsections; i++){
            if(section->sh_type==SHT_REL){
                relocate=(Elf32_Rel*)(map_start + section->sh_offset);
                if(debug_mode==true)
                    printf("Relocation at offset %0x contains %d entries:\n",section->sh_offset,section->sh_size/section->sh_entsize);
                printf("Offset\t\t Info\n");
                for(j=0;j<section->sh_size/section->sh_entsize;j++){
                    printf("%08x\t %08x\n",relocate->r_offset,relocate->r_info);
                    relocate++;
                }
            }
        section++;
        }
    } else fprintf(stderr, "No file currently open!");
}

void relocate_tables_semantic()
{
    Elf32_Shdr* section;
    Elf32_Shdr* sectionheader;
    Elf32_Shdr symbol_table;
    Elf32_Rel* relocate;
    Elf32_Sym* symboltable_offset;
    char * symbol_str_table;
    int i,j,type,symname;
    if(Currentfd!=FILE_CLOSED)
    {
        sectionheader=(Elf32_Shdr*)(map_start+header->e_shoff);
        section=(Elf32_Shdr*)(map_start+header->e_shoff);
        for( i = 0; i < numofsections; i++)
        {
            if(section->sh_type==SHT_REL)
            {
                relocate=(Elf32_Rel*)(map_start + section->sh_offset);
                symbol_table = (Elf32_Shdr) sectionheader[section->sh_link]; /* symbol table section header */
                symbol_str_table = (char *) (map_start + sectionheader[symbol_table.sh_link].sh_offset); /* string table of the associated symbol table */
                symboltable_offset=(Elf32_Sym*)(map_start+sectionheader[section->sh_link].sh_offset);
                printf("Relocation section %s at offset %0x contains %d entries:\n",string_table+section->sh_name,section->sh_offset,section->sh_size/section->sh_entsize);
                printf("Offset\t\t Info\t\t Type\t Sym.Value\t Sym. Name\n");
                for(j=0;j<section->sh_size/section->sh_entsize;j++){
                    type=ELF32_R_TYPE(relocate->r_info);
                    symname=ELF32_R_SYM(relocate->r_info);
                    printf("%08x\t %08x\t %d\t %08x\t %s\n",relocate->r_offset,relocate->r_info,type,symboltable_offset->st_value,symbol_str_table+symboltable_offset[symname].st_name);
                    relocate++;
                }
                printf("\n");
            }
           section++;
        }
    } else fprintf(stderr, "No file currently open!");
}