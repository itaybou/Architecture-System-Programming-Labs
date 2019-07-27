#include <stdio.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <zconf.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#define MAX_LINE_SIZE 2048
#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

int toList=0;   //flag that tells if the command goes to process list

typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

static process* processesList=NULL; /*list of processes*/

int isDebugMode=0;  /*a flag tells if -d flag encountered in the command*/

int execute(cmdLine*);
void infinithLoop();
int launch(cmdLine *pLine);
void deleteProcessFromList(int index);  /*delete the process located in index place in the list*/

int cd(cmdLine*);   /*emulates the cd shell feature*/
int procs(cmdLine*);
void freeProcessList(process*);    //free list of processes
void updateProcessList(process **); //update terminated processes status
void updateProcessStatus(process* , int , int );
/*shell features utilities*/
char* shellFeaturesStr[]={"cd","procs"};
int (*shellFeaturesFunc[])(cmdLine*)={&cd,&procs};
int numOfShellFeatures(){
    return sizeof(shellFeaturesStr)/ sizeof(char*);
}
void addProcess(process**, cmdLine*, pid_t); /*add a process to list*/
void printProcessList(process**);  /*print all processes in list*/

int main(int argc, char **argv) {
    int i;
    /*determine if debuf flag is set on cmd line*/
    for(i=0;i<argc;i++){
        if(strcmp(argv[i],"-d")==0){
            isDebugMode=1;
            break;
        }
    }


    infinithLoop();

    return 0;   /*exit normally*/
}



/**
 * this is the infinith loop that:
 * read, parse and execute the user commands
 */
void infinithLoop(){
    FILE* input = stdin;
    cmdLine *pcmd_line;
    char strLine[MAX_LINE_SIZE];
    int status;
    char cwdPath[PATH_MAX];


    do{

        getcwd(cwdPath,PATH_MAX);               /*get current dir path*/
        fprintf(stdout,"%s ",cwdPath);          /*print the path*/
        toList=0;
        fgets(strLine,MAX_LINE_SIZE,input);     /*read*/
        pcmd_line=parseCmdLines(strLine);       /*parse*/

        status=execute(pcmd_line);              /*execute*/

        //free allocated
        if(!toList)
            freeCmdLines(pcmd_line);

    }while(status);
    freeProcessList(processesList);
}
/**
 * responsible to execute the user command
 */
int execute(cmdLine* pCmdLine){
    int i;
    if (pCmdLine->arguments[0] == NULL) {
        /* An empty command was entered.*/
        return 1;
    }
    /*check if user requested to quit*/
    if(strcmp(pCmdLine->arguments[0],"quit")==0){
        return 0;
    }

    for(i=0;i<numOfShellFeatures();i++){
        if(strcmp(pCmdLine->arguments[0],shellFeaturesStr[i])==0){
            return (*shellFeaturesFunc[i])(pCmdLine);
        }
    }

    return launch(pCmdLine);

}

int launch(cmdLine *pLine) {

    pid_t curPid;
    int status;

    curPid=fork(); /*clone process*/

    if(isDebugMode&&curPid!=0){
        fprintf(stderr,"PID: %d\nExecuting command: %s\n",curPid,pLine->arguments[0]);
    }

    if(curPid==0){  /*run the command from the child process*/

        int execRet=execvp(pLine->arguments[0],pLine->arguments);
        if(isDebugMode){
            fprintf(stderr,"PID: %d\nExecuting command: %s\n",curPid,pLine->arguments[0]);
        }
        if(execRet==-1){
            perror("error in execution attempt");
            _exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }
    else if(curPid<0){
        perror("error forking");
    }else{
        toList=1;
        addProcess(&processesList,pLine,curPid);
        if(pLine->blocking==1){
            do{
                waitpid(curPid,&status,WUNTRACED);
            }while(!WIFEXITED(status)&&!WIFSIGNALED((status)));
        }

    }

    return 1;   /*tells the calling function to prompt for input again*/
}

int cd(cmdLine* pcmdLine){
    if(pcmdLine->arguments[1]==NULL){
        fprintf(stderr,"destination dir is missing for cd command\n");
    } else{ /*execute cd command*/
        int resExec=chdir(pcmdLine->arguments[1]);
        if(resExec!=0){
            fprintf(stderr,"Couldn't go to %s\n",pcmdLine->arguments[1]);
        }
    }
    return 1;
}

int procs(cmdLine* pcmdLine){
    if(processesList==NULL){
        fprintf(stderr,"no active processes to print.\n");
    } else{
        printProcessList(&processesList);
    }
    return 1;
}

void addProcess(process** process_list, cmdLine* gcmd, pid_t gpid){
    process* p=*process_list;   /*p points to the head of list*/

    /*create a new node*/
    process* newNode=(process*)malloc(sizeof(process));
    newNode->next=NULL;
    newNode->cmd=gcmd;
    newNode->pid=gpid;
    newNode->status=RUNNING;
    /*list is null*/
    if(*process_list==NULL){
        *process_list=newNode;
    }
    else {
        /*p pointer will point to the last node*/
        while (p->next != NULL) {
            p = p->next;
        }
        /*update the new node to be the last node*/
        p->next = newNode;
    }
}

void printProcessList(process** process_list){
    updateProcessList(process_list);
    int updatedIndex=0;
    printf("Index      |      Process Id |      Status |      Command\n");
    process* p = *process_list;
    int i=0;
    while(p!=NULL){
        char* statusStr;
        char* commandStr;
        if(p->status==RUNNING){
            statusStr=(char*)malloc(8);
            strcpy(statusStr,"RUNNING");
        }
        else if(p->status==TERMINATED){
            statusStr=(char*)malloc(11);
            strcpy(statusStr,"TERMINATED");
        } else{
            statusStr=(char*)malloc(10);
            strcpy(statusStr,"SUSPENDED");
        }
        int j=0;
        commandStr=(char*)malloc(1);
        commandStr[0]='\0';
        //get the command
        while(p->cmd->arguments[j]!=NULL){
            commandStr=(char*)realloc(commandStr,sizeof(commandStr)+strlen(p->cmd->arguments[j])+2);
            strcat(commandStr," ");
            strcat(commandStr,p->cmd->arguments[j]);
            j++;
        }
        printf("%d%20d%20s%15s\n",i,p->pid,statusStr,commandStr);
        //free allocatde
        free(statusStr);
        free(commandStr);
        //in case of "freshly" terminated, delete the process from list
        if(p->status==TERMINATED){
            deleteProcessFromList(updatedIndex);
            updatedIndex--;
        }
        i++;
        updatedIndex++;
        p=p->next;
    }
}

void freeProcessList(process* process_list){
    process* p=process_list;
    while(process_list!=NULL){
        process_list=process_list->next;
        freeCmdLines(p->cmd);
        free(p);
        p=process_list;
    }
}

void updateProcessList(process **process_list){
    process* p=*process_list;
    while(p!=NULL){
       int status;
       if(waitpid(p->pid,&status,WNOHANG)==TERMINATED){
           updateProcessStatus(processesList,p->pid,TERMINATED);
       }
       p=p->next;
    }
}
/* find the process with the given id in the process_list and change its status to the received status.*/
void updateProcessStatus(process* process_list, int pid, int status){
    process* p=process_list;
    while(p!=NULL){
            if(p->pid==pid){
                p->status=status;
                break;
            }
            p=p->next;
    }
}
void deleteProcessFromList(int index){
    int i=0;
    process* p=processesList;
    if(index==0){   //the node to delete is the head
        processesList=processesList->next;
        freeCmdLines(p->cmd);
        free(p);
    } else{//index>0 so go to the prev of the node we wish to delete
        while(i!=index-1){
            p=p->next;
            i++;
        }
        //here p=prev(nodeToDelete)
        process* cur=p->next;
        p->next=cur->next;  //update linking of list
        //delete Node
        freeCmdLines(cur->cmd);
        free(cur);
    }
}
