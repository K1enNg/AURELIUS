#include <stdio.h>

void swap(int *a, int *b);
void calculateStats(float a, float b, float c, float *sum, float *avr, float *max);
float findMax(float a, float b, float c);

int main() {
    int num = 43;
    int *ptr = &num; // 'ptr' holds memory address of 'num'
    printf("%d\n", ptr);

    int val = *ptr; // '*ptr' dereferences 'ptr', returning 43
    printf("%d\n\n", val);

    int x = 10;

    //declaring a pointer and assigning an address
    int *ptr1 = &x;

    printf("value of x: %d\n", x);
    printf("memory of x: %p\n", (void*)&x);
    printf("value stored in ptr1: %p\n", (void*)ptr1);
    printf("value pointed to (*ptr): %d\n\n", *ptr);

    //pass-by-reference func demo
    int num1 = 1, num2 = 2;
    printf("before swap: num1: %d, num2: %d\n", num1, num2);

    swap(&num1, &num2);
    printf("after swap: num1: %d, num2: %d\n", num1, num2);

    //practice
    float a, b, c;
    float sum = 0;
    float average = 0;
    float max = 0;

    printf("enter 3 inputs (separated by spaces): ");
    scanf("%f %f %f", &a, &b, &c); // &f for float

    calculateStats(a, b, c, &sum, &average, &max);
    printf("sum: %f\n", sum);
    printf("average: %f\n", average);
    printf("max: %f\n\n", max);

    return 0;
}

void swap(int *a, int *b) {
    int temp = *a; //store value at 'a'
    *a = *b; // copy value 'b' to 'a'
    *b = temp; //copy temp to 'b' 
}

void calculateStats(float a, float b, float c, float *sum, float *avr, float *max) {
    *sum = a + b + c;
    *avr = *sum/3.0f;
    *max = findMax(a, b ,c);
    return;
}

float findMax(float a, float b, float c) {
    if (a > b && a > c) {
        return a;
    } else if (b > a && b > c) {
        return b;
    } else {
        return c;
    }
}