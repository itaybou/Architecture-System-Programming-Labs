#flags
CC = gcc
CFLAGS  = -g -m32 -Wall -c -o
OBJS = addresses.o

# All Targets
all: lab2b

# Tool invocations
lab2b: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -m32 -Wall -o addresses $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
addresses.o: addresses.c
	$(CC) $(CFLAGS) addresses.o addresses.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab1d
