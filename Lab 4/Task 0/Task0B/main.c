#include "util.h"

#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define STDOUT 1
#define EXIT 1

#define SEEK_CUR 1
#define O_RDRW 2

int main (int argc , char* argv[], char* envp[])
{
   int desc, replace_pos = 0x296 - strlen("Shira"); /*Little endian so we subtract*/
   char * filename, *name;
   if(argc != 3)
      system_call(EXIT, 0x55)

   filename = argv[1];
   name = argv[2];

   desc = system_call(SYS_OPEN, filename, O_RDRW, 0777);
   system_call(SYS_LSEEK, desc, replace_pos, SEEK_CUR);
   if((system_call(SYS_WRITE, desc, name, strlen(name))) == -1) {
      system_call(SYS_CLOSE, desc);
      system_call(EXIT, 0x55);
   }
   system_call(SYS_WRITE, desc, ".\n", 2);
   system_call(SYS_CLOSE, desc);
  return 0;
}
