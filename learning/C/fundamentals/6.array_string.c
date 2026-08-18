#include <stdio.h>
#include <string.h>

void reverseString(char *str);

int main() {
    int numbers[4] = {1, 2, 3, 4};
    int *ptr = numbers;

    for (int i = 0; i < 4; i++) {
        //accessing using array indexing vs. pointer arithmetic
        printf("index %d: array syntax = %d | pointer syntax = %d | address = %d\n",
                i,          numbers[i],         *(ptr + i),         (void*)(ptr + i));
    }

    char greeting[] = "Hello";
    char *charPtr = greeting;

    printf("string content: %s\n", greeting);

    while(*charPtr != '\0') {
        printf("%c ", *charPtr);
        charPtr++;
    }

    printf("\n");

    //practice
    char text[99];

    printf("print a word to reverse: ");
    scanf("%99s", text);

    reverseString(text);

    printf("reversed: %s\n", text);

    return 0;
}

void reverseString(char *str) {
    if (str == NULL || *str == '\0') {
        return; // check
    }

    char *start = str;
    char *end = str + strlen(str) - 1; //pointer to last char before '\0'

    while(start < end) {
        //swap start and end using temp char variable
        char temp = *start;
        *start = *end;
        *end = temp;

        //move pointers towards the middle using pointer arithmetic
        start++;
        end--;
    }


}