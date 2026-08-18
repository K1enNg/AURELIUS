#include <stdio.h>

int main() {
    //variables
    int age = 25;
    float height = 5.9f;
    double pi = 3.14;
    char grade = 'A';

    printf("Age: %d \n", age);
    printf("Height: %d \n", height);
    printf("Pi: %d \n", pi);
    printf("Grade: %d \n", grade);


    //memory size
    printf("size of int: %lu \n", sizeof(int));
    printf("size of float: %lu \n", sizeof(float));
    printf("size of double: %lu \n", sizeof(double));
    printf("size of char: %lu \n", sizeof(char));

    return 0;
}