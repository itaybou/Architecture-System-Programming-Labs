
# All Targets
all: task0

# Tool invocations
task0: skeleton.o
	ld -m elf_i386  skeleton.o -o task0
	
# Depends on the source and header files
skeleton.o: skeleton.s
	nasm -f elf32 skeleton.s -o skeleton.o

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o task0
