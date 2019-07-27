#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <linux/limits.h>

#include "LineParser.h"

typedef int bool;
#define true 1
#define false 0

#define ERROR -1
#define SUCCESS 0

#define NOP 2

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0

#define MAX_INPUT_LEN 2048 * sizeof(char)

typedef struct command_func {
  char *command;
  bool (*function)(cmdLine*);
} command_func;

typedef struct process {
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

bool execute(cmdLine *pCmdLine);
bool check_shell_command(cmdLine* pCmdLine);
void parse_flags(int arg_count, char ** args);
void print_debug(int pid, char * const* proc_args, int arg_count);
char * get_process_str_from_status(int status);
bool checkIfTerminatedProc(process * proc);

void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);
void freeProcessList(process* process_list);
void updateProcessList(process **process_list);
void updateProcessStatus(process* process_list, int pid, int status);

//Shell commands
bool print_processes(cmdLine* pCmdLine);
bool change_dir(cmdLine* pCmdLine);
bool quit(cmdLine* pCmdLine);


bool debug = false; // Determines if the program is in debug mode.
struct command_func commands[] = {{ "quit", &quit }, { "cd", &change_dir }, { "procs", &print_processes } };
process * processes; // Process list

int main (int argc, char** argv)
{
   char wd_path[PATH_MAX];
   char input [MAX_INPUT_LEN];
   cmdLine * parsed_cmd_line;

   bool quit = false;
   processes = NULL;
   parse_flags(argc, argv);

   while(quit == false)
   {
      getcwd(wd_path, PATH_MAX);
      printf("%s> ", wd_path);
      fgets(input, MAX_INPUT_LEN, stdin);
      if (strcmp(input, "\n") != 0) {
         parsed_cmd_line = parseCmdLines(input);
         quit = execute(parsed_cmd_line);
      }
   }
   freeProcessList(processes);
   exit(EXIT_SUCCESS);
}

void parse_flags(int arg_count, char ** args)
{
   int i;
   for(i = 0; i < arg_count; ++i)
      if(strcmp(args[i], "-d") == 0)
         debug = true;
}

bool execute(cmdLine *pCmdLine)
{
   pid_t pid;
   int wstatus = 0;
   bool ret_val;

   ret_val = check_shell_command(pCmdLine);
   if(ret_val != NOP)
      return ret_val;

   pid = fork();
   if(pid != 0 && pid != ERROR) {
      addProcess(&processes, pCmdLine, pid);
      print_debug(pid, (*pCmdLine).arguments, (*pCmdLine).argCount);
      if((*pCmdLine).blocking == 1)
         do waitpid(pid, &wstatus, WUNTRACED);
         while(!WIFSIGNALED(wstatus) && !WIFEXITED(wstatus));
   }
   if (pid == ERROR)
      perror("Error while creating a proccess fork");
   else if (pid == 0) { // Try to exec only if you are the child proccess
      if(execvp((*pCmdLine).arguments[0], (*pCmdLine).arguments) == ERROR) {
         perror("Error executing proccess");
         _exit(EXIT_FAILURE); // exit "abnormally"
      }
      exit(EXIT_SUCCESS);
   }
   return false;
}

bool check_shell_command(cmdLine* pCmdLine)
{
   int i;
   bool ret_val = NOP;
   const int commands_size = sizeof(commands)/sizeof(struct command_func);
   for(i = 0; i < commands_size; ++i)
   {
      if(strcmp((*pCmdLine).arguments[0], commands[i].command) == 0) {
         ret_val = (commands[i].function)(pCmdLine);
         freeCmdLines(pCmdLine);
         break;
      }
   }
   return ret_val;
}

void addProcess(process** process_list, cmdLine* gcmd, pid_t pid)
{
   process * proc, * curr;
   proc = (process *)malloc(sizeof(struct process));
   (*proc).cmd = gcmd;
   (*proc).pid = pid;
   (*proc).status = RUNNING;
   (*proc).next = NULL;

   curr = *process_list;

   if (curr != NULL)
   {
      while((*curr).next != NULL)
         curr = (*curr).next;
      (*curr).next = proc;
   } else *process_list = proc;
}

void printProcessList(process** process_list)
{
   int index = 1, command_len = 0, i;
   char * command;
   process * curr = *process_list, *temp;

   updateProcessList(process_list);
   if(curr == NULL)
      printf("No processes exists.\n");
   else {
      printf("#\tPID\t\tSTATUS\t\tCommand\n");
      while(curr != NULL)
      {
         for(i = 0; i < (*curr).cmd->argCount; ++i)
            command_len += strlen((*curr).cmd->arguments[i]) + 1;

         command = (char *)calloc(command_len, sizeof(char));
         for(i = 0; i < (*curr).cmd->argCount; ++i) {
            if(i != 0)
               strcat(command, " ");
            strcat(command, (*curr).cmd->arguments[i]);
         }
         printf("%d\t%d\t\t%s\t\t%s\n", index, (*curr).pid, get_process_str_from_status((*curr).status), command);
         free(command);
         temp = (*curr).next;
         if(checkIfTerminatedProc(curr))
            --index;
         curr = temp;
         ++index;
      }
   }
}

void freeProcessList(process* process_list)
{
   process * curr = process_list;
   while (curr != NULL)
   {
      process_list = (*process_list).next;
      freeCmdLines((*curr).cmd);
      free(curr);
      curr = process_list;
   }
}

void updateProcessList(process **process_list)
{
   process * curr = *process_list;
   int wstatus = RUNNING;

   while(curr != NULL)
   {
      wstatus = waitpid((*curr).pid, &wstatus, WNOHANG);
      if(wstatus == TERMINATED)
            updateProcessStatus(*process_list, (*curr).pid, TERMINATED);
      curr = (*curr).next;
   }
}

void updateProcessStatus(process* process_list, int pid, int status)
{
   process * curr = process_list;

   while(curr != NULL)
   {
      if((*curr).pid == pid)
         (*curr).status = status;
      curr = (*curr).next;
   }
}

bool checkIfTerminatedProc(process * proc)
{
   process * curr = processes, *next, *prev = NULL;
   int i = 0;

   if((*proc).status == TERMINATED)
   {
      while (curr != NULL)
      {
         if((*curr).pid == (*proc).pid)
         {
            if(i == 0) //First in process list
            {
               next = (*curr).next;
               freeCmdLines((*curr).cmd);
               free(curr);
               curr = next;
               processes = curr;
               break;
            }
            else {
               (*prev).next = (*curr).next;
               freeCmdLines((*curr).cmd);
               free(curr);
               break;
            }
         } else {
            prev = curr;
            curr = (*curr).next;
         }
         ++i;
      }
      return true;
   } else return false;
}

char * get_process_str_from_status(int status)
{
   switch (status) {
      case SUSPENDED:
         return "Suspended";
      case TERMINATED:
         return "Terminated";
      case RUNNING:
         return "Running";
   }
   return NULL;
}

bool print_processes(cmdLine* pCmdLine)
{
   updateProcessList(&processes);
   printProcessList(&processes);
   return false;
}

bool change_dir(cmdLine* pCmdLine)
{
   char * path;
   int i;

   if((*pCmdLine).argCount >= 2)
   {
      path = (char *)calloc(PATH_MAX, sizeof(char));
      for(i = 1; i < (*pCmdLine).argCount; ++i) {
         if(i != 1)
            strcat(path, " ");
         strcat(path, (*pCmdLine).arguments[i]);
      }
      if(chdir(path) == ERROR)
         fprintf(stderr, "Given path is illegal or does not exist. %s\n", path);
      free(path);
   }
   else fprintf(stderr, "Illegal change dir command, missing directory to change to. (command: cd <DIR>)\n");
   return false;
}

bool quit(cmdLine* pCmdLine)
{
   return true;
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
