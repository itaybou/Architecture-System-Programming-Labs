#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = looper.o

# All Targets
all: looper

# Tool invocations
looper: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o looper $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
looper.o: looper.c
	$(CC) $(CFLAGS) looper.o looper.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o looper
