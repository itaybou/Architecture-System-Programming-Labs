/*
 * Submitter:
 * Name: Itay Bouganim
 * ID: 305278384
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

#define EXIT_SUCC 0
#define EXIT_ERR 1

char toLower (char ch);

int main (int argc, char ** argv)
{
    FILE* input = stdin, *output=stdout; //I/O Files, default points to stdin and stdout
    int i, enc_ind=0; // Indexes
    char *encrypt; //Encryption string
    char ch, temp; //Holds current character
    bool debug = false, enc_plus = false, enc_minus = false; // Argument flags
      const int two_char = 2*sizeof(char); // Two char size - used for parsing
    
    //Parse command line arguments
    for(i=1 ; i<argc; i++)
    {
        if(strncmp(argv[i], "-i", two_char) == 0)
            input = fopen(argv[i]+two_char, "r"); //Point to input name file
        else if(strcmp(argv[i], "-D") == 0) {
            fprintf(stderr, "%s\n", argv[i]);
            debug = true;
        }
        else if((strncmp(argv[i], "+e", two_char) == 0) ^ (strncmp(argv[i], "-e", two_char) == 0))
        {
            //Update encryption type flags
            enc_plus = argv[i][0] == '+';
            enc_minus = argv[i][0] == '-';
            encrypt =(char *)malloc(strlen(argv[i]+two_char) * sizeof(char)); //Allocate string space for encryption string.
            strcpy(encrypt, argv[i]+two_char); //copy argument encryption string
        }
        else {
            printf("Invalid argument parameter given: %s\nExiting.\n", argv[i]);
            return EXIT_ERR;
        }
    }
    
    //Read input characters and print to output file
    ch = getc(input);
    while(ch != EOF)
    {
        temp = ch;
        fprintf(stderr, (debug == true) ? "0x%02x\t" : "", ch); // Prints hexadecimal representation to Stderr in debug mode
        if((enc_plus == true) ^ (enc_minus == true))
            ch = enc_plus == true ? ch+encrypt[enc_ind] : ch-encrypt[enc_ind]; // Add or Subtract read char to curr encryption string char
        else ch = toLower(ch);
        fprintf(stderr, (debug == true) ? "0x%02x\n" : "", ch); // Prints hexadecimal representation to Stderr in debug mode
        fputc(ch, output);
        if((enc_plus == true) ^ (enc_minus == true) && temp == '\n')
        {
            enc_ind = -1;
            fprintf(output, "%c", temp); //Used to print \n at the end of encrypted input
        }
        ch = fgetc(input);
        enc_ind = ((enc_plus == true) ^ (enc_minus == true) && enc_ind == strlen(encrypt)-1) ? 0 : enc_ind+1;
    }
    //Free encryption string mem allocated
    if(encrypt)
        free(encrypt);
    //Close input and output files
    fclose(input);
    if(output != stdout)
        fclose(output);
    return EXIT_SUCC;
}

/*
 * Converts upper case characters to lower case
 */
char toLower(char ch)
{
    if(ch>='A' && ch<='Z')
        ch += 32;
    return ch;
}

