#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

#include "LineParser.h"

typedef int bool;
#define true 1
#define false 0

#define ERROR -1

#define MAX_INPUT_LEN 2048 * sizeof(char)

bool execute(cmdLine *pCmdLine);
void parse_flags(int arg_count, char ** args);
void print_debug(int pid, char * const* proc_args, int arg_count);

bool debug = false;

int main (int argc, char** argv)
{
   char wd_path[PATH_MAX];
   char input [MAX_INPUT_LEN];
   cmdLine * parsed_cmd_line;

   bool quit = false;
   parse_flags(argc, argv);

   while(quit == false)
   {
      getcwd(wd_path, PATH_MAX);
      printf("%s> ", wd_path);
      fgets(input, MAX_INPUT_LEN, stdin);
      if (strcmp(input, "\n") != 0) {
         parsed_cmd_line = parseCmdLines(input);
         quit = execute(parsed_cmd_line);
         freeCmdLines(parsed_cmd_line);
      }
   }

   exit(EXIT_SUCCESS);
}

void parse_flags(int arg_count, char ** args)
{
   int i;
   for(i = 0; i < arg_count; ++i)
   {
      if(strcmp(args[i], "-d") == 0)
         debug = true;
   }
}

bool execute(cmdLine *pCmdLine)
{
   pid_t pid;

   if(strcmp((*pCmdLine).arguments[0], "quit") == 0)
      return true;

   pid = fork();
   if(pid != 0)
      print_debug(pid, (*pCmdLine).arguments, (*pCmdLine).argCount);
   if (pid == ERROR)
      perror("Error while duplicating proccess");
   else if (pid == 0) { // Try to exec only if you are the child proccess
      if(execvp((*pCmdLine).arguments[0], (*pCmdLine).arguments) == ERROR) {
         perror("Error executing proccess");
         _exit(EXIT_FAILURE); // exit "abnormally"
      }
      exit(EXIT_SUCCESS);
   }

   return false;
}

void print_debug(int pid, char * const* proc_args, int arg_count)
{
   int i;
   if(debug == true)
   {
      fprintf(stderr, "DEBUG:\nCurrent PID: %d, Executing command: %s\nArguments: ", pid, proc_args[0]);
      for(i = 1; i < arg_count; ++i) {
         if(i != arg_count - 1)
            fprintf(stderr, "%s [%d], ", proc_args[i], i);
         else fprintf(stderr, "%s [%d]\n", proc_args[i], i);
      }
   }
}
