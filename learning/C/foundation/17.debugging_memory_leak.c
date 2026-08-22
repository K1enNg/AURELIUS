#include <stdio.h>
#include <stdlib.h>

// void createStudent(int **arr, int size) {
//     // BUG 1: Allocation made, but pointer logic is wrong
//     *arr = (int *)malloc(size * sizeof(int));
//     for (int i = 0; i <= size; i++) { // BUG 2: Out of bounds write (i <= size)
//         (*arr)[i] = (i + 1) * 10;
//     }
// }

// int main() {
//     int *data = NULL;
//     createStudent(&data, 5);

//     printf("Value at index 0: %d\n", data[0]);

//     // BUG 3: Memory leak — free(data) is omitted before exit
//     return 0;
// }

// FIX:
void createStudent(int **arr, int size) {
    *arr = (int *)malloc(size * sizeof(int));
    if (*arr) return;
    for (int i = 0; i < size; i++) { // BUG 2: Out of bounds write (i <= size)
        (*arr)[i] = (i + 1) * 10;
    }
}

int main() {
    int *data = NULL;
    createStudent(&data, 5);

    printf("Value at index 0: %d\n", data[0]);

    free(data);
    return 0;
}