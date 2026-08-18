#include <stdio.h>

int main() {
    //conditional statement
    int score;
    printf("input score num: ");
    scanf("%d", &score);

    if (score >= 90) {
        printf("A\n");
    } else if (score >= 80) {
        printf("B\n");
    } else {
        printf("C\n");
    }

    //simple calculation
    int num1;
    int num2;
    char operator;
    int result = 0;

    printf("enter num 1: ");
    scanf("%d",&num1);

    printf("enter operator: ");
    scanf(" %c", &operator); // using %c operator is char

    printf("enter num 2: ");
    scanf("%d",&num2);

    if (operator == '+') {
        result = num1 + num2;
    } else if (operator == '-') {
        result = num1 - num2;
    } else if (operator == '*') {
        result = num1 * num2;
    } else if (operator == '/') {
        if (num2 != 0){
            result = num1 / num2;
        } else {
            printf("error: division by zero");
        }
    } else {
        printf("error: invalid operation");
    }

    printf("result: %d\n", result);

    //loops
    //1. for loop
    printf("for loop demo\n");
    for (int i = 1; i <= 5; i++) {
        printf("%d\n", i);
    }

    //2. while loop
    printf("while loop demo\n");
    int countdown = 3;
    while (countdown > 0) {
        printf("%d\n", countdown);
        countdown--;
    }
    //3. do-while loop
    printf("do-while loop demo\n");
    int num = 0;
    do {
        printf("%d\n", num);
        num++;
    } while (num < 5);

    return 0;
}