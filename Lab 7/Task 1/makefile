#flags
CC = gcc
CFLAGS  = -g -Wall -c -o
OBJS = hexeditplus.o

# All Targets
all: hexedit

# Tool invocations
hexedit: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -Wall -o hexeditplus $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
hexeditplus.o: hexeditplus.c
	$(CC) $(CFLAGS) hexeditplus.o hexeditplus.c

.PHONY: clean

#Clean the build directory
clean:
	rm -f *.o hexeditplus
