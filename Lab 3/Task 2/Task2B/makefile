#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = task2b.o

# All Targets
all: lab3

# Tool invocations
lab3: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o task2b $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
task2b.o: task2b.c
	$(CC) $(CFLAGS) task2b.o task2b.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o task2b
