#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = task1c.o

# All Targets
all: lab3

# Tool invocations
lab3: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o task1c $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
task1c.o: task1c.c
	$(CC) $(CFLAGS) task1c.o task1c.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab3
