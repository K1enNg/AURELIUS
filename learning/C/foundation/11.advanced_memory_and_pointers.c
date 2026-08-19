#include <stdio.h>
#include <stdlib.h>
#include "string.h"

// 1. function pointers
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

void computeAndPrint(int x, int y, int (*operation)(int, int)) {
    int result = operation(x, y);
    printf("result: %d\n", result);
}

// 2.void pointers (genetic pointers)
void printGenericValue(void *data, char type) {
    switch(type) {
        case 'i':
            printf("integer: %d\n", *(int *)data);
            break;
        case 'f':
            printf("float: %.2f\n", *(float *)data);\
            break;
        case 'c':
            printf("char: %c\n", *(char *)data);
            break;
    }
}

//3. practice
typedef struct {
    int id;
    float gpa;
} Student;

// comparator callback for qsort (sort ascending by GPA)
int compareByGpa(const void *a, const void *b) {
    const Student *s1 = (const Student *)a;
    const Student *s2 = (const Student *)b;

    if (s1->gpa > s2->gpa) return -1;
    if (s2->gpa > s1->gpa) return 1;

    return 0;
}

int main() {
    
    //1. declare func pointer matching 'int func(int, int)'
    int (*opPtr)(int, int);

    opPtr = add;
    computeAndPrint(10, 5, opPtr);

    opPtr = multiply;
    computeAndPrint(10, 5, opPtr);

    //2. declare void pointer
    int num = 42;
    float pi = 3.14f;

    printGenericValue(&num, 'i');
    printGenericValue(&pi, 'f');

    //3. practice
    //3.1 using qsort with func pointers and void*
    Student class[] = {
        {101, 3.2f},
        {102, 3.7f},
        {103, 2.6f},
    };

    int count = sizeof(class)/sizeof(class[0]);

    //qsort accepts void* data array and a comparison func pointer
    qsort(class, count, sizeof(Student), compareByGpa);

    for (int i = 0; i < count; i++) {
        printf("\nid: %d, gpa: %.2f\n", class[i].id, class[i].gpa);
    }

    //3.2 direct raw memory operations: memcpy and memset
    //memset : Fills n bytes of memory with value c
    //memcpy : Copies n bytes from src to dest (non-overlapping)
    Student backup[3];

    memcpy(backup, class, sizeof(class));
    printf("backup copied. id[0]: %d\n", backup[0].id);

    memset(backup, 0, sizeof(backup));
    printf("After memset zeroing: ID[0]: %d | GPA[0]: %.2f\n", backup[0].id, backup[0].gpa);
    
    return 0;
}