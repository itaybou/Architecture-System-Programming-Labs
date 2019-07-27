#flags
CC = gcc
CFLAGS  = -g -m32 -Wall -c -o
OBJS = 1d.o

# All Targets
all: lab

# Tool invocations
lab: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -m32 -Wall -o 1d $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
1d.o: 1d.c
	$(CC) $(CFLAGS) 1d.o 1d.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab1d
