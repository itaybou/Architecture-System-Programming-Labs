#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = task1a.o

# All Targets
all: lab3

# Tool invocations
lab3: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o task1a $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
task1a.o: task1a.c
	$(CC) $(CFLAGS) task1a.o task1a.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab3
