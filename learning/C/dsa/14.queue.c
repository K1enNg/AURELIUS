#include <stdio.h>
#include <stdlib.h>

// Queue (FIFO - First In, First Out): Enqueue at rear, Dequeue from front. Think of a line at a store.

typedef struct QueueNode {
    int data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} LinkedQueue;

LinkedQueue *createQueue();
void enqueue(LinkedQueue *q, int value);
int dequeue(LinkedQueue *q);
void freeQueue(LinkedQueue *q);

int main() {
    LinkedQueue *queue =createQueue();
    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);

    printf("dequeue: %d\n", dequeue(queue));
    printf("dequeue: %d\n", dequeue(queue));
    freeQueue(queue);
    return 0;
}

LinkedQueue *createQueue() {
    LinkedQueue *q =(LinkedQueue *)malloc(sizeof(LinkedQueue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

void enqueue(LinkedQueue *q, int value) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    newNode->data = value;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    printf("enqueued: %d\n", value);
}

int dequeue(LinkedQueue *q) {
    if (!q->front) {
        return -1;
    }
    QueueNode *temp = q->front;
    int val = temp->data;

    q->front = q->front->next;
    if (!q->front) {!q->rear;}
    free(temp);
    return val;
}

void freeQueue(LinkedQueue *q) {
    QueueNode *curr = q->front;
    while (curr) {
        QueueNode *next = curr->next;
        free(q);
        curr = next;
    }
    free(q);
}