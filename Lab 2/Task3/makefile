#flags
CC = gcc
CFLAGS  = -g -m32 -Wall -c -o
OBJS = menu_map.o

# All Targets
all: lab2

# Tool invocations
lab2: $(OBJS)
	@echo 'Invoking Linker'
	$(CC) -g -m32 -Wall -o menu_map $(OBJS)
	@echo 'Finished building target.'
	@echo ' '

# Depends on the source and header files
menu_map.o: menu_map.c
	$(CC) $(CFLAGS) menu_map.o menu_map.c

.PHONY: clean
	
#Clean the build directory
clean: 
	rm -f *.o menu_map
