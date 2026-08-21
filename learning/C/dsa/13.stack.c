#include <stdio.h>
#include <stdlib.h>

// Stack (LIFO - Last In, First Out): Push to top, Pop from top. Think of a stack of plates.

typedef struct {
    int *data, top, capacity;
} ArrayStack;

ArrayStack *createStack(int inititialCapacity);
void push(ArrayStack *stack, int value);
int pop(ArrayStack *stack);
int peek(const ArrayStack *stack);
void freeStack(ArrayStack *stack);

int main() {
    ArrayStack *stack = createStack(2);
    push(stack, 10);
    push(stack, 20);
    push(stack, 30);

    printf("peek top: %d\n", peek(stack));
    printf("popped: %d\n", pop(stack));
    printf("popped: %d\n", pop(stack));
    freeStack(stack);
    return 0;
}

ArrayStack *createStack(int inititialCapacity) {
    ArrayStack *stack = (ArrayStack *)malloc(sizeof(ArrayStack));
    stack->data = (int *)malloc(inititialCapacity * sizeof(int));
    stack->top = -1;
    return stack;
}

void push(ArrayStack *stack, int value) {
    if(stack->top == stack->capacity - 1) {
        stack->capacity *= 2;
        stack->data = (int *)realloc(stack->data, stack->capacity * sizeof(int));
    }
    stack->data[++(stack->top)] = value;
    printf("[stack] pushed: %d\n", value);
}

int pop(ArrayStack *stack) {
    if (stack->top == -1) return -1;
    return stack->data[(stack->top)--];
}

int peek(const ArrayStack *stack) {
    if (stack->top == -1) return -1;
    return stack->data[stack->top];
}

void freeStack(ArrayStack *stack) {
    free(stack->data);
    free(stack);
}