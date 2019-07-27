#include <stdio.h>
#include <stdlib.h>

void bubbleSort(int numbers[], int array_size) {
    int i, j;
    int *temp;
    for (i = (array_size - 1); i > 0; i--) {
        for (j = 1; j <= i; j++) {
            if (numbers[j - 1] > numbers[j]) {
                temp = (int *) malloc(sizeof(int *));
                *temp = numbers[j - 1];
                numbers[j - 1] = numbers[j];
                numbers[j] = *temp;
				free(temp); // int * temp is allocated and need to freed each iteration
            }
        }
    }
}

int main(int argc, char **argv) {
    char **arr = argv + 1;
    int i, n = argc - 1;
    int *numbers = (int *) calloc(n, sizeof(int));

    printf("Original array:");
    for (i = 0; i < n; ++i) { //Needed to remove <= n and start i from 0 to avoid segfault
        printf(" %s", arr[i]);
        numbers[i] = atoi(arr[i]);
    }
    printf("\n");

    bubbleSort(numbers, n);

    printf("Sorted array:");
    for (i = 0; i < n; ++i)
        printf(" %d", numbers[i]);
    printf("\n");
	
	free(numbers); // numbers array need to be freed since allocated and use is finished. 

    return 0;
}
