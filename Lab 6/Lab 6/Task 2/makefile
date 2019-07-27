#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = mypipeline.o

# All Targets
all: shell

# Tool invocations
shell: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o mypipeline $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
mypipeline.o: mypipeline.c
	$(CC) $(CFLAGS) mypipeline.o mypipeline.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o mypipeline
