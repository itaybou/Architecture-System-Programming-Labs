#include "util.h"

typedef int bool;
#define true 1
#define false 0

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define SYS_GETDENTS 141
#define SYS_EXIT 1

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define SEEK_CUR 1
#define SEEK_END 2

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDRW 2
#define O_CREAT 64

#define MAX_LEN 50
#define BUFF_SIZE 8192

typedef struct ent {
   int inode;  /* Inode number */
   int offset; /* Offset from begining of directory */
   unsigned short len;  /* Length of this entry */
   char dir_name[]; /* Filename (null-terminated) flexible*/
} ent;

char *input_file = "stdin", *output_file = "stdout";
char sys_id, ret_code;
bool debug = false;
int input_desc = STDIN, output_desc = STDOUT;

char toLower(char ch);
void print_debug(void);
void error_quit(char * msg);

int main (int argc , char* argv[], char* envp[])
{
   int wd = 0;
   char buffer[BUFF_SIZE];
   struct ent *entp;
   int bpos;
   int nread;
   int i;

   /* Parse program arguments */
   for(i=1 ;i<argc; i++) {
      if(strcmp(argv[1], "-D") == 0)
         debug = true;
   }

   if((wd = system_call(SYS_OPEN, ".", O_RDONLY, 0)) < 0) /* Open current working dir */
      error_quit("Error opening current working directory! EXITING");
   print_debug();

   nread = system_call(SYS_GETDENTS, wd, buffer, BUFF_SIZE);
   ret_code = system_call(sys_id = SYS_WRITE, output_desc, "Filename:\t\tLength:\n", strlen("Filename:\t\tLength:\n"));
   for (bpos = 0; bpos < nread;) {
       entp = (struct ent *) (buffer + bpos);
       system_call(sys_id = SYS_WRITE, output_desc, entp->dir_name, strlen(entp->dir_name));
       system_call(sys_id = SYS_WRITE, output_desc, "\t\t\t", 3);
       ret_code = system_call(sys_id = SYS_WRITE, output_desc, itoa(entp->len), strlen(itoa(entp->len)));
       system_call(sys_id = SYS_WRITE, output_desc, "\n", 1);
       bpos += entp->len;
    }

   system_call(SYS_EXIT, 0);
   return 0;
}

void print_debug(void)
{
   char *sys_call_str, *ret_code_str, *in, *out;
   if(debug) {
      in = "\nInput file: ";
      out = "\nOutput file: ";
      sys_call_str = "\nSystem call ID: ";
      ret_code_str = "\nReturn Code: ";
      system_call(SYS_WRITE, STDERR, in, strlen(in));
      system_call(SYS_WRITE, STDERR, input_file, strlen(input_file));
      system_call(SYS_WRITE, STDERR, out, strlen(out));
      system_call(SYS_WRITE, STDERR, output_file, strlen(output_file));
      system_call(SYS_WRITE, STDERR, sys_call_str, strlen(sys_call_str));
      system_call(SYS_WRITE, STDERR, itoa(sys_id), 2);
      system_call(SYS_WRITE, STDERR, ret_code_str, strlen(ret_code_str));
      system_call(SYS_WRITE, STDERR, itoa(ret_code), 2);
      system_call(SYS_WRITE, STDERR, "\n", 1);
   }
}

void error_quit(char * msg)
{
   ret_code = system_call(sys_id = SYS_WRITE, STDERR, msg, strlen(msg));
   print_debug();
   system_call(SYS_EXIT, 0x55);
}
