#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = task1b.o

# All Targets
all: lab3

# Tool invocations
lab3: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o task1b $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
task1b.o: task1b.c
	$(CC) $(CFLAGS) task1b.o task1b.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab3
