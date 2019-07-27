# All Targets
all: myELF

# Tool invocations
myELF: Task3.o
	gcc -g -m32  Task3.o -o myELF


# Depends on the source and header files
Task3.o: Task3.c
	gcc -g -m32 -Wall -ansi -c  Task3.c -o Task3.o

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o myELF
