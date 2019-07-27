# define some Makefile variables for the compiler and compiler flags
CC = gcc
ASM = nasm
ASMFLAGS = -g -f elf -w+all -o
CFLAGS  = -g -m32 -Wall -c -o
OBJS = main.o numbers.o add.o

# All Targets
all: ass0

# Tool invocations
ass0: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -m32 -Wall -o ass0 $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
main.o: main.c
	$(CC) $(CFLAGS) main.o main.c

# Depends on the source and header files
numbers.o: numbers.c
	$(CC) $(CFLAGS) numbers.o numbers.c
	
# Depends on the source and header files
add.o: add.s
	$(ASM) $(ASMFLAGS) add.o add.s

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o ass0
