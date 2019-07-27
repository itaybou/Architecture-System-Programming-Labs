#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef int bool;
#define true 1
#define false 0

#define CHILD 0

#define STDIN_FD 0
#define STDOUT_FD 1

void parse_flags(int arg_count, char ** args);
void initCommands(char **, char **);
void print_debug(char * message, pid_t pid);

bool debug = false; // Determines if the program is in debug mode.

int main (int argc , char* argv[])
{
   parse_flags(argc, argv);
   int fd[2], proc1, proc2, stat; //Pipe ends
   char * command1[3];
   char * command2[4];
   initCommands(command1, command2);
   pipe(fd);
   print_debug("(parent_process>forking…)\n", 0);
   proc1 = fork();

   if(proc1 != CHILD)
   {
      print_debug("(parent_process>created process with id: %d)\n", proc1);
      print_debug("(parent_process>closing the write end of the pipe…)\n", 0);
      close(fd[1]);
      proc2 = fork();
      if(proc2 != CHILD)
      {
         print_debug("(parent_process>closing the read end of the pipe…)\n", 0);
         close(fd[0]);
         print_debug("(parent_process>waiting for child processes to terminate…)\n", 0);
         do waitpid(proc1, &stat, WUNTRACED);
         while(!WIFSIGNALED(stat) && !WIFEXITED(stat));
         do waitpid(proc2, &stat, WUNTRACED);
         while(!WIFSIGNALED(stat) && !WIFEXITED(stat));
      }
      else {
         print_debug("(child2>redirecting stdin to the write end of the pipe…)\n", 0);
         close(STDIN_FD);
         dup(fd[0]);
         close(fd[0]);
         print_debug("(child2>going to execute cmd: …)\n", 0);
         execvp(command2[0], command2);
      }
   } else {
      print_debug("(child1>redirecting stdout to the write end of the pipe…)\n", 0);
      close(STDOUT_FD);
      dup(fd[1]);
      close(fd[1]);
      print_debug("(child1>going to execute cmd: …)\n", 0);
      execvp(command1[0], command1);
   }

   print_debug("(parent_process>exiting…)\n", 0);
   return 0;
}

void print_debug(char * message, pid_t pid)
{
   if(debug)
      fprintf(stderr, message, pid);
}

void initCommands(char ** command1, char ** command2)
{
   //Init command 1 arguments
   command1[0] = "ls";
   command1[1] = "-l";
   command1[2] = NULL;

   //Init command 2 arguments
   command2[0] = "tail";
   command2[1] = "-n";
   command2[2] = "2";
   command2[3] = NULL;
}

void parse_flags(int arg_count, char ** args)
{
   int i;
   for(i = 0; i < arg_count; ++i)
      if(strcmp(args[i], "-d") == 0)
         debug = true;
}
