#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char encrypt(char c);
char decrypt(char c);
char xprt(char c);
char cprt(char c);
char my_get(char c);
char quit(char c);

char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}

char toLower(char c) {
	if(c >= 'A' && c<='Z')
		c += 32;
	return c;
}
 
char* map(char *array, int array_length, char (*f) (char))
{
	char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
	/* TODO: Complete during task 2.a */
	int i;
	for(i=0; i<array_length; i++)
		*(mapped_array+i) = (*f)(*(array+i));
	return mapped_array;
}
 
int main(int argc, char **argv) {
	/* TODO: Test your code */
	char *arr = "HEY MAN!";
	const int arr_size = strlen(arr);
	int i;
	char * out1 = map(arr, arr_size, &censor);
	char * out2 = map(arr, arr_size, &toLower);
  
	printf("Input is: \n");
	for(i=0; i<arr_size; i++)
		printf("%c", *(arr+i));
	
	printf("\nMapped using censor function: \n");
	for(i=0; i<arr_size; i++)
		printf("%c", *(out1+i));
  
	printf("\nMapped using toLower function: \n");
	for(i=0; i<arr_size; i++)
	  printf("%c", *(out2+i));
	
	printf("\n\n");
	int base_len = 5;
	char arr1[base_len];
	char* arr2 = map(arr1, base_len, my_get);
	char* arr3 = map(arr2, base_len, encrypt);
	char* arr4 = map(arr3, base_len, xprt);
	char* arr5 = map(arr4, base_len, decrypt);
	char* arr6 = map(arr5, base_len, cprt);
	
	printf("\nEncrypted: \n");
	for(i=0; i<base_len; i++)
		printf("%c", *(arr3+i));
	
	printf("\nDecrypted: \n");
	for(i=0; i<base_len; i++)
	  printf("%c", *(arr5+i));
	
	free(arr2);
	free(arr3);
	free(arr4);
	free(arr5);
	free(arr6);
	
	free(out1);
	free(out2);

}

char encrypt (char c)
{
	if(c>=0x20 && c<=0x7E)
		return c+3;
	else return c;
}

char decrypt (char c)
{
	if(c>=0x20 && c<=0x7E)
		return c-3;
	else return c;
}

char xprt (char c) 
{
	printf("0x%x\n", c);
	return c;
}

char cprt (char c) 
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

char quit (char c)
{
	exit(0);
}
