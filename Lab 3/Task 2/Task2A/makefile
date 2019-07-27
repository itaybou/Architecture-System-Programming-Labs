#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = task2a.o

# All Targets
all: lab3

# Tool invocations
lab3: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o task2a $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
task2a.o: task2a.c
	$(CC) $(CFLAGS) task2a.o task2a.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab3
