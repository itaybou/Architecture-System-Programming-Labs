#include "util.h"

typedef int bool;
#define true 1
#define false 0

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19
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

char *input_file = "stdin", *output_file = "stdout";
char sys_id, ret_code;
bool debug = false;
int input_desc = STDIN, output_desc = STDOUT;

char toLower(char ch);
void print_debug(void);
void error_quit(char * msg);

int main (int argc , char* argv[], char* envp[])
{
   char ch;
   int i;

   /* Parse program arguments */
   for(i=1 ;i<argc; i++) {
      if(strcmp(argv[i], "-D") == 0)
         debug = true;
      else if(strncmp(argv[i], "-i", 2*sizeof(char)) == 0) {
         input_file = argv[i]+2;
         if((ret_code = input_desc = system_call(sys_id = SYS_OPEN, input_file, O_RDONLY, 0777)) < 0)
            error_quit("\nError opening input file! EXITING.\n");
         print_debug();
      }
      else if(strncmp(argv[i], "-o", 2*sizeof(char)) == 0) {
         output_file = argv[i]+2;
         if((ret_code = output_desc = system_call(sys_id = SYS_OPEN, output_file, O_WRONLY | O_CREAT, 0777)) < 0)
            error_quit("\nError opening output file! EXITING.\n");
         print_debug();
      }
   }

   /* Get input and print output char by char loop */
   while ((ret_code = system_call(sys_id = SYS_READ, input_desc, &ch, 1)) > 0) /* while ch != EOF */
   {
      print_debug();
      ch = toLower(ch);
      ret_code = system_call(sys_id = SYS_WRITE, output_desc, &ch, 1);
      print_debug();
   }

   ret_code = system_call(sys_id = SYS_CLOSE, input_desc);
   print_debug();
   if(strcmp(output_file, "stdout") != 0) {
      ret_code = system_call(sys_id = SYS_CLOSE, output_desc);
      print_debug();
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

char toLower(char ch)
{
   if(ch>='A' && ch<='Z')
        ch += 32;
   return ch;
}
