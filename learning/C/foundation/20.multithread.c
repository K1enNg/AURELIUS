// By default, a standard C program executes sequentially as a single thread within a single process. 
// Multithreading allows a program to spawn multiple concurrent execution threads that run simultaneously, 
// sharing the same memory space (heap, global variables, file descriptors).

//producer-consumer problem
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#define QUEUE_CAPACITY 5
#define NUM_PRODUCERS  2
#define NUM_CONSUMERS  2
#define ITEMS_PER_PRODUCER 10

typedef struct {
    int data[QUEUE_CAPACITY];
    int head;
    int tail;
    int count;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} BoundedQueue;

BoundedQueue *queue;

BoundedQueue *createQueue();
void queue_push(BoundedQueue *q, int item);
int queue_pop(BoundedQueue *q);
void destroyQueue(BoundedQueue *q);
void* producer_thread(void *arg);
void* consumer_thread(void *arg);

int main() {
    queue = createQueue();

    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    for (long i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producers[i], NULL, producer_thread, (void *)(uintptr_t)i);
    }

    for (long i = 0;i < NUM_CONSUMERS; i++) {
        pthread_create(&consumers[i], NULL, consumer_thread, (void *)(uintptr_t)i);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) pthread_join(producers[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) pthread_join(consumers[i], NULL);

    destroyQueue(queue);

    return 0;
}

BoundedQueue *createQueue() {
    BoundedQueue *q = (BoundedQueue *)malloc(sizeof(BoundedQueue));
    q->head = 0;
    q->tail = 0;
    q->count = 0;

    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);

    return q;
}

void queue_push(BoundedQueue *q, int item) {
    pthread_mutex_lock(&q->mutex);

    while (q->count == QUEUE_CAPACITY) {
        pthread_cond_wait(&q->not_full, &q->mutex);
    }

    q->data[q->tail] = item;
    q->tail = (q->tail + 1) % QUEUE_CAPACITY;
    q->count++;

    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);
}

int queue_pop(BoundedQueue *q) {
    pthread_mutex_lock(&q->mutex);

    while (q->count == 0) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }

    int item = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_CAPACITY;
    q->count--;

    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
    return item;
}

void destroyQueue(BoundedQueue *q) {
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
    free(q);
}

void* producer_thread(void *arg) {
    long id = (long)(intptr_t)arg;
    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int item = (id * 100) * i;
        queue_push(queue, item);
        printf("[Producer %ld] Pushed: %d\n", id, item);
        usleep(50000); // simulate work delay
    }
}

void* consumer_thread(void *arg) {
    long id = (long)(intptr_t)arg;
    int total_to_consume = (NUM_PRODUCERS * ITEMS_PER_PRODUCER) / NUM_CONSUMERS;
    for (int i = 0; i < total_to_consume; i++) {
        int item = queue_pop(queue);
        usleep(100000);
    }
    pthread_exit(NULL);
}
