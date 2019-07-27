#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = hexaPrint.o

# All Targets
all: lab3

# Tool invocations
lab3: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o hexaPrint $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
hexaPrint.o: hexaPrint.c
	$(CC) $(CFLAGS) hexaPrint.o hexaPrint.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o lab3
