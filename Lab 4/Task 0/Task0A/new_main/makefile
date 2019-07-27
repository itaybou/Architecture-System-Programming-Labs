# define some Makefile variables for the compiler and compiler flags
CC = gcc
ASM = nasm
LINKER = ld
ASMFLAGS = -g -f elf
CFLAGS  = -m32 -Wall -ansi -c -nostdlib -fno-stack-protector
LINKERFLAGS = -m elf_i386
OBJS = start.o main.o util.o

# All Targets
all: lab4

# Tool invocations
lab4: $(OBJS)
	@echo 'Invoking Linker'
	$(LINKER) $(LINKERFLAGS) $(OBJS) -o task0
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

# Depends on the source and header files
util.o: util.c
	$(CC) $(CFLAGS) util.c -o util.o

# Depends on the source and header files
start.o: start.s
	$(ASM) $(ASMFLAGS) start.s -o start.o
	

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o task0
