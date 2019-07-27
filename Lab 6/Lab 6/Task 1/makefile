#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = task1.o LineParser.o

# All Targets
all: shell

# Tool invocations
shell: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o myshell $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
task1.o: task1.c
	$(CC) $(CFLAGS) task1.o task1.c

LineParser.o: LineParser.c
	$(CC) $(CFLAGS) LineParser.o LineParser.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o myshell
