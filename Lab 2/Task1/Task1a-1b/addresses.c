#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5; //Allocated in Unintialized data at compilation
int addr6; //Allocated in Unintialized data at compilation

int foo(); //Allocated in text at compilation
void point_at(void *p); //Allocated in text at compilation

int main (int argc, char** argv){ //Arguments allocated in stack at runtime
    int addr2; //Allocated at stack at runtime
    int addr3; //Allocated at stack at runtime
    char* yos="ree"; //Pointer allocated in compilation value allocated at stack at runtime
    int * addr4 = (int*)(malloc(50)); //Pointer allocated at compilation Value allocated at Heap at runtime
    printf("- &addr2: %p\n",&addr2);
    printf("- &addr3: %p\n",&addr3);
    printf("- foo: %p\n",foo);
    printf("- &addr5: %p\n",&addr5);
    
	point_at(&addr5);
	
    printf("- &addr6: %p\n",&addr6);
    printf("- yos: %p\n",yos);
    printf("- addr4: %p\n",addr4);
    printf("- &addr4: %p\n",&addr4);
    return 0;
}

int foo(){
    return -1;
}

void point_at(void *p){ //Arguments allocated in stack at runtime
    int local; //Allocated at stack at runtime
	static int addr0 = 2; //Declared static therefore allocated at Initialized data at compilation
    static int addr1; //Declared static therefore allocated at Uninitialized data at compilation


    long dist1 = (size_t)&addr6 - (size_t)p;
    long dist2 = (size_t)&local - (size_t)p;
    long dist3 = (size_t)&foo - (size_t)p;
    
    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3);
	
	printf("- addr0: %p\n", & addr0);
    printf("- addr1: %p\n",&addr1);
}

