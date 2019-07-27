#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1

int main (int argc , char* argv[], char* envp[])
{
   char * str = "Hello World!\n";
   system_call(SYS_WRITE, STDOUT, str, strlen(str));
  return 0;
}
