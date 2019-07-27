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

#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12
#define DT_WHT 14

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define SEEK_CUR 1
#define SEEK_END 2

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDRW 2
#define O_CREAT 64

#define BUFF_SIZE 8192

typedef struct ent {
   int inode;  /* Inode number */
   int offset; /* Offset from begining of directory */
   unsigned short len;  /* Length of this entry */
   char name[]; /* Filename (null-terminated) */
} ent;

int sys_id, ret_code;
bool debug = false, prefix = false, infect = false;
char *prefix_val;

void parse_arguments(int arg_count, char* argv[]);
void iterate_entries(struct ent *entp, char *buffer, int nread);
void print_entry(struct ent *entp, int file_type);
void infect_entry(struct ent *entp);
bool check_flag_prefix(char * ent_name, bool flag);
char * get_ent_file_type(char file_type);
void print_debug(void);
void error_quit(char * msg);

int main (int argc , char* argv[], char* envp[])
{
   int wd = 0; /* Working directory descriptor */
   char buffer[BUFF_SIZE];
   char *title = "Filename:\t\tType:\n";
   struct ent *entp;
   int nread;

   parse_arguments(argc, argv); /* Parse program arguments */

   if((wd = system_call(sys_id = SYS_OPEN, ".", O_RDONLY, 0)) < 0) /* Open current working dir */
      error_quit("Error opening current working directory! EXITING");
   ret_code = wd;
   print_debug();

   nread = system_call(sys_id = SYS_GETDENTS, wd, buffer, BUFF_SIZE); /*Get entries data to buffer */
   ret_code = nread;
   print_debug();

   if(!debug)
      ret_code = system_call(sys_id = SYS_WRITE, STDOUT, title, strlen(title));
   iterate_entries(entp, buffer, nread);

    if((ret_code = system_call(sys_id = SYS_CLOSE, wd)) < 0) /* Open current working dir */
      error_quit("Error closing current working directory! EXITING");
   print_debug();

   system_call(SYS_EXIT, 0);
   return 0;
}

void iterate_entries(struct ent *entp, char *buffer, int nread)
{
   int buff_pos,file_type;

   for (buff_pos = 0; buff_pos < nread;) {
      entp = (struct ent *) (buffer + buff_pos);
      buff_pos += entp->len;
      file_type = buffer[buff_pos-1];
      if(check_flag_prefix(entp->name, infect)) {
         print_entry(entp, file_type);
         infect_entry(entp);
      }
      else if((!infect && !prefix) || check_flag_prefix(entp->name, prefix))
         print_entry(entp, file_type);
   }
}

void print_entry(struct ent *entp, int file_type)
{
   char *f_type;

   ret_code = system_call(sys_id = SYS_WRITE, STDOUT, entp->name, strlen(entp->name));
   print_debug();
   if(debug) {
      ret_code = system_call(sys_id = SYS_WRITE, STDOUT, itoa(entp->len), strlen(itoa(entp->len)));
      print_debug();
   } else ret_code = system_call(sys_id = SYS_WRITE, STDOUT, "\t\t\t", 3);
   f_type = get_ent_file_type(file_type);
   ret_code = system_call(sys_id = SYS_WRITE, STDOUT, f_type, strlen(f_type));
   print_debug();
   if(!debug)
      ret_code = system_call(sys_id = SYS_WRITE, STDOUT, "\n", 1);
}

void infect_entry(struct ent *entp)
{
   infector(entp->name);
}

bool check_flag_prefix(char * ent_name, bool flag)
{
   return flag && ((strlen(ent_name) - strlen(prefix_val)) >= 0)
         && (strncmp(ent_name, prefix_val, strlen(prefix_val)) == 0);
}

void parse_arguments(int arg_count, char* argv[])
{
   int i;
   for(i=1 ;i<arg_count; i++) {
      if(strcmp(argv[i], "-D") == 0)
         debug = true;
      else if(strncmp(argv[i], "-p", 2) == 0) {
         prefix = true;
         if(strlen(argv[i]+2) > 10)
            error_quit("Prefix parameter exceeds 10 characters bound! EXITING");
         else prefix_val = argv[i]+2;
      }
      else if(strncmp(argv[i], "-a", 2) == 0) {
         infect = true;
         if(strlen(argv[i]+2) > 10)
            error_quit("Prefix parameter exceeds 10 characters bound! EXITING");
         else prefix_val = argv[i]+2;
      }
      else error_quit("Invalid parameter given! EXITING\n");
   }
}

void print_debug(void)
{
   char *sys_call_str, *ret_code_str;
   if(debug) {
      sys_call_str = "\nSystem call ID: ";
      ret_code_str = "\tReturn Code: ";
      system_call(SYS_WRITE, STDERR, sys_call_str, strlen(sys_call_str));
      system_call(SYS_WRITE, STDERR, itoa(sys_id), strlen(itoa(sys_id)));
      system_call(SYS_WRITE, STDERR, ret_code_str, strlen(ret_code_str));
      system_call(SYS_WRITE, STDERR, itoa(ret_code), strlen(itoa(ret_code)));
      system_call(SYS_WRITE, STDERR, "\n", 1);
   }
}

void error_quit(char * msg)
{
   ret_code = system_call(sys_id = SYS_WRITE, STDERR, msg, strlen(msg));
   print_debug();
   system_call(SYS_EXIT, 0x55);
}

char * get_ent_file_type(char file_type)
{
   return (file_type == DT_REG) ?  "regular" :
          (file_type == DT_DIR) ?  "directory" :
          (file_type == DT_FIFO) ? "FIFO" :
          (file_type == DT_SOCK) ? "socket" :
          (file_type == DT_LNK) ?  "symlink" :
          (file_type == DT_BLK) ?  "block dev" :
          (file_type == DT_CHR) ?  "char dev" : "???";
}
