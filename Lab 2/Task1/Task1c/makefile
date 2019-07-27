#flags
CC = gcc
CFLAGS  = -g -m32 -Wall -c -o
OBJS = 1c.o

# All Targets
all: lab

# Tool invocations
lab: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -m32 -Wall -o 1c $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
1c.o: 1c.c
	$(CC) $(CFLAGS) 1c.o 1c.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab1d
