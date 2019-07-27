# All Targets
all: myELF

# Tool invocations
myELF: myELF.o
	gcc -g -m32  myELF.o -o myELF


# Depends on the source and header files
myELF.o: myELF.c
	gcc -g -m32 -Wall -ansi -c  myELF.c -o myELF.o

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o myELF

