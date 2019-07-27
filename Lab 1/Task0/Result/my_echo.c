#include <stdio.h>
#include <stdlib.h>

#define EXIT_SUCC 0

int main(int argc, char *argv[])
{
	for(int i=1; i<argc; i++)
	{
		printf("%s ", argv[i]);
	}
	return EXIT_SUCC;
}
