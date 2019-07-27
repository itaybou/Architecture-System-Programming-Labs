#flags
CC = gcc
CFLAGS  = -g -m32 -Wall -c -o
OBJS = count-words.o

# All Targets
all: lab2

# Tool invocations
lab2: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -m32 -Wall -o count $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
count-words.o: count-words.c
	$(CC) $(CFLAGS) count-words.o count-words.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab1d
