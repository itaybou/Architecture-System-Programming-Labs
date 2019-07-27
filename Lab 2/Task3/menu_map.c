#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

#define ARR_SIZE(arr) sizeof(arr)/sizeof(arr[0])
#define BASE_LEN 5

void print_menu(void);
char* map(char *array, int array_length, char (*f) (char));
void flush_stdin(void);

char censor(char c);
char encrypt(char c);
char decrypt(char c);
char xprt(char c);
char cprt(char c);
char my_get(char c);
char quit(char c);

struct fun_desc {
  char *name;
  char (*fun)(char);
};

struct fun_desc menu[] = { { "Censor", &censor }, { "Encrypt", encrypt },
							{ "Decrypt", &decrypt }, { "Print hex", &xprt },
							{ "Print string", &cprt }, { "Get String", &my_get },
							{ "Junk", &menu }, { "Quit", &quit }, { NULL, NULL } };
							
 
int main(int argc, char **argv) {
	char arr[BASE_LEN] = ""; //Initialize array of size BASE_LEN to \0
	char *temp, *carray = arr;
	bool pointed = false;

	int option;
	char *choice = (char*)malloc(16*sizeof(char));
	const int menu_size = ARR_SIZE(menu)-2; //Ignoring the NULL option and zero index
	while(true) 
	{
		printf("Please choose a function:\n");
		print_menu();
		printf("Option: ");
		
		flush_stdin();
		choice = fgets(choice, 16*sizeof(char), stdin);
		option = atoi(choice);
		
		if((*choice != '0' && option == 0)) { //Inserted illegal string containing digits other than numbers
			printf("Not within bounds\n\n");
			if(pointed == true)
				free(carray);
			free(choice);
			quit('0');
		}
		else if(option >=0 && option<=menu_size) {
				
			printf("Within bounds\n");
			temp = map(carray, BASE_LEN, menu[option].fun);
			flush_stdin();
			if(pointed == true)
				free(carray);
			carray = temp;
			pointed = true;
			printf("DONE.\n\n");
		}
		else {
			printf("Not within bounds\n\n");
			if(pointed == true)
				free(carray);
			free(choice);
			quit('0');
		}
	}
}

void flush_stdin(void)
{
	char ch;
	ungetc('\n', stdin);
	ch = fgetc(stdin);
	while (ch != '\n' && ch != EOF);
}

char* map(char *array, int array_length, char (*f) (char))
{
	int i;
	char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
	for(i=0; i<array_length; i++)
		*(mapped_array+i) = (*f)(*(array+i));
	return mapped_array;
}

void print_menu(void)
{
	int i = 0;
	while(menu[i].name != NULL) 
	{
		printf("%i) %s\n",i , menu[i].name);
		++i;
	}
}

char censor(char c) {
  if(c == '!')
    return '.';
  else return c;
}

char encrypt(char c)
{
	if(c>=0x20 && c<=0x7E)
		return c+3;
	else return c;
}

char decrypt(char c)
{
	if(c>=0x20 && c<=0x7E)
		return c-3;
	else return c;
}

char xprt(char c) 
{
	printf("0x%x\n", c);
	return c;
}

char cprt(char c) 
{
	if(c>=0x20 && c<=0x7E)
		printf("%c\n", c);
	else printf(".\n");
	return c;
}

char my_get(char c)
{
	return fgetc(stdin);
}

char quit(char c)
{
	exit(0);
}
