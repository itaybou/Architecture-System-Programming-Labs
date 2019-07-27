# define some Makefile variables for the compiler and compiler flags
CC = gcc
CFLAGS  = -g -m32 -Wall -c -o
OBJS = encoder.o

# All Targets
all: lab1

# Tool invocations
lab1: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -m32 -Wall -o lab1 $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
encoder.o: encoder.c
	$(CC) $(CFLAGS) encoder.o encoder.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab1
