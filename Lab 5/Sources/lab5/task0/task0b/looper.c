#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>

void sigHandler(int);

int main(int argc, char **argv){

    //printf("Starting the program\n");
    /*register the signal handler*/
    signal(SIGINT,sigHandler);
	signal(SIGTSTP,sigHandler);
	signal(SIGCONT,sigHandler);

    while(1) {
        
        sleep(2);
    }
    return 0;
}

void sigHandler(int sigNum){
    printf(" Looper handling signal: %s\n",strsignal(sigNum));
    /*the following states that the coming signals will walk thru my handler*/
    if(sigNum==SIGCONT){
		signal(SIGINT,sigHandler);
	}
	else if(sigNum==SIGINT){
		signal(SIGCONT,sigHandler);
	}
    /*forward the received signal to the default handler*/
    signal(sigNum,SIG_DFL);
    raise(sigNum);
}
