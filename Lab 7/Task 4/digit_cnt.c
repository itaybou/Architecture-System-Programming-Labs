#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
   if(argc == 1)
      printf("usage: ./ntsc <string>\n");
   else printf("The number of digits in the string is: %d\n", (unsigned int)strlen(argv[1]));
    return 0;
}
