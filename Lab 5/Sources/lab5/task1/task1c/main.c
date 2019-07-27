#include <stdio.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <zconf.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#define MAX_LINE_SIZE 2048


int isDebugMode=0;  /*a flag tells if -d flag encountered in the command*/

int execute(cmdLine*);
void infinithLoop();
int launch(cmdLine *pLine);

int cd(cmdLine*);   /*emulates the cd shell feature*/

/*shell features utilities*/
char* shellFeaturesStr[]={"cd"};
int (*shellFeaturesFunc[])(cmdLine*)={&cd};
int numOfShellFeatures(){
    return sizeof(shellFeaturesStr)/ sizeof(char*);
}


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

        fgets(strLine,MAX_LINE_SIZE,input);     /*read*/
        pcmd_line=parseCmdLines(strLine);       /*parse*/
        status=execute(pcmd_line);              /*execute*/

        //free allocated
        freeCmdLines(pcmd_line);

    }while(status);
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
