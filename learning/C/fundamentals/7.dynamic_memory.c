#include <stdio.h>
#include <stdlib.h>

int main() {

    //4 core functions
    //
    //malloc(): allocates uninitialized memory block
    //calloc(): allocates memory block and clears all bytes to 0
    //realloc(): resizes an existing allocated memory block
    //free(): releases allocated memory back to the operating system

    //1. malloc: allocate space for n integers
    int *arr = (int *)malloc(n * sizeof(int));

    if (arr = NULL) {
        printf("memory allocation failed")
        return 1;
    }

    for (int i = 0; i < n, i++) {
        arr[i] = (i + 1) * 10;
    }

    for (int i = 0; i < n, i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    //2. realloc: expand the arraya from 3 elements to 5 elements
    int new_n = 5;
    int *temp = (int *)realloc(arr, new_n * sizeof(int));

    if (temp == NULL) {
        printf("reallocation failed");
        free(arr);
        return 1;
    }
    arr = temp; //reassign after NULL check

    arr[3] = 40;
    arr[4] = 50;

    //3. free: clean up heap memory
    free(arr);
    arr = NULL;
    printf("memory freed")

    return 0;
}