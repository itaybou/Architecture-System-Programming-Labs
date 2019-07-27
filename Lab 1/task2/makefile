#flags
CC = gcc
CFLAGS  = -g -m32 -Wall -c -o
OBJS = encoder.o

# All Targets
all: lab1d

# Tool invocations
lab1d: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -m32 -Wall -o encoder $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
encoder.o: encoder.c
	$(CC) $(CFLAGS) encoder.o encoder.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab1d
