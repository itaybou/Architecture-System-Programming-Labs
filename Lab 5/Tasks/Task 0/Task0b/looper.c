#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void signal_handler(int signal_desc);

int main(int argc, char **argv){

	//Assign signal handler to SIGSTP, SIGINT, SIGCONT
	signal(SIGTSTP, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGCONT, signal_handler);

	while(1) {
		sleep(2);
	}

	return 0;
}

void signal_handler (int signal_desc)
{
	printf("\n%s signal received.\n", strsignal(signal_desc));
	switch(signal_desc)
	{
		case SIGCONT:
			signal(SIGINT, signal_handler);
			break;
		case SIGINT:
			signal(SIGCONT, signal_handler);
			break;
	}
	signal(signal_desc, SIG_DFL);
	raise(signal_desc);
}
