#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

#define EXIT_SUCC 0
#define EXIT_ERR 1

char toLower(char ch);

int main (int argc, char ** argv)
{
	int i, ch, enc_ind = 0;
	FILE *output = stdout, *input = stdin;
	bool debug=false, enc_plus=false, enc_minus=false;
	char encrypt[]="", temp;
	const int twoChar = 2*sizeof(char);
	//Parse command flags
	for(i=1; i<argc; i++) {

		if(strncmp(argv[i], "-o", twoChar)==0)
			output = fopen(argv[i]+twoChar, "w");

		else if(strncmp(argv[i], "-i", twoChar)==0)
			input = fopen(argv[i]+twoChar, "r");

		else if(strcmp(argv[i],"-D")==0) {
			fprintf(stderr, "%s\n", argv[i]);
			debug = true;
		}
 		else if (strncmp(argv[i],"+e", twoChar)==0 || strncmp(argv[i],"-e", twoChar)==0)
		{
			enc_plus = argv[i][0] == '+';
			enc_minus = argv[i][0] == '-';
			strcpy(encrypt, argv[i]+twoChar);
		}
		else {
			printf("Invalid Parameter given: %s\n", argv[i]);
			return EXIT_ERR;
		}
	}

	ch = getc(input);
	while(ch != EOF)
	{
		temp = ch;
		if(debug == true)
			fprintf(stderr, "0x%02x\t", (unsigned int)ch);
		if((enc_plus == true) ^ (enc_minus == true))
		{
			ch = (enc_plus == true) ? ch+(unsigned int)encrypt[enc_ind] : ch-(unsigned int)encrypt[enc_ind];
		}
		else ch = toLower(ch);
		if(debug == true)
			fprintf(stderr, "0x%02x\n", (unsigned int)ch);
		fputc(ch, output);
		if(((enc_plus == true) ^ (enc_minus == true)) && temp == '\n')
		{
			enc_ind = -1;
			fprintf(output, "%c", temp);
		}
		ch = fgetc(input);
		enc_ind = (((enc_plus == true) ^ (enc_minus == true)) && enc_ind==strlen(encrypt)-1) ? 0 : enc_ind+1;
	}
	fclose(input);
	if(output != stdout)
		fclose(output);
	return EXIT_SUCC;
}

char toLower(char ch)
{
	if(ch>='A' && ch<='Z')
		ch += 32;
	return ch;
}

