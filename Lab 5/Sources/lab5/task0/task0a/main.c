#include <stdio.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <zconf.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#define MAX_LINE_SIZE 2048





int execute(cmdLine*);
void infinithLoop();
int launch(cmdLine *pLine);

int main(int argc, char **argv) {

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
        // An empty command was entered.
        return 1;
    }
/*
    for (i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
*/
    if(strcmp(pCmdLine->arguments[0],"quit")==0){
          return 0;
    }
    return launch(pCmdLine);

}

int launch(cmdLine *pLine) {

    pid_t curPid,wpid;
    int status;

    curPid=fork(); /*clone process*/
    if(curPid==0){
        if(execvp(pLine->arguments[0],pLine->arguments)==-1){
            perror("error in execution attempt");
        }
        exit(EXIT_FAILURE);
    }
    else if(curPid<0){
        perror("error forking");
    }else{
        do{
            wpid=waitpid(curPid,&status,WUNTRACED);
        }while(!WIFEXITED(status)&&!WIFSIGNALED((status)));
    }

    return 1;   /*tells the calling function to prompt for input again*/
}