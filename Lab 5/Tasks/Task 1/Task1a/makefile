#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = myshell.o LineParser.o

# All Targets
all: shell

# Tool invocations
shell: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o myshell $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
myshell.o: myshell.c
	$(CC) $(CFLAGS) myshell.o myshell.c

LineParser.o: LineParser.c
	$(CC) $(CFLAGS) LineParser.o LineParser.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o myshell
