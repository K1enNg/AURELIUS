#include <stdio.h>

int add(int a, int b);
void demonstrateScope();
void attemptModification(int x);

int globalCounter = 0;

int main() {
    int val1 = 10, val2 = 20;

    int sum = add(val1, val2);
    printf("sum of %d and %d is: %d\n", val1, val2, sum);

    printf("original value before func: %d\n", val1);
    attemptModification(val1);

    printf("original value after func: %d\n", val1);
    demonstrateScope();
    printf("global counter accessed in main: %d\n", globalCounter);
    return 0;
}

int add(int a, int b) {
    return a + b;
}

void attemptModification(int x) {
    x = 999;
    printf("value changed to %d\n", x);
}

void demonstrateScope() {
    int localVar = 100;
    globalCounter++;
    printf("local is %d, global is %d\n", localVar, globalCounter);
}