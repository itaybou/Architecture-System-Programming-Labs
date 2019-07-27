#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = bubblesort.o

# All Targets
all: lab3

# Tool invocations
lab3: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o bubblesort $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
bubblesort.o: bubblesort.c
	$(CC) $(CFLAGS) bubblesort.o bubblesort.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab3
