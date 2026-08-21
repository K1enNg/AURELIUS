#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

typedef struct Node {
    char *key;
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *buckets[TABLE_SIZE];
} HashTable;

unsigned int hash(const char *key);
HashTable* createTable();
void insert(HashTable *table, const char *key, int value);
int get(HashTable *table, const char *key, int *outValue);
int deleteKey(HashTable *table, const char *key);
void printTable(HashTable *table);
void freeTable(HashTable *table);

int main() {
    HashTable *ht = createTable();
    
    // Insert keys (some will collide into the same bucket)
    insert(ht, "Alice", 95);
    insert(ht, "Bob", 88);
    insert(ht, "Charlie", 72);
    insert(ht, "David", 90);
    insert(ht, "Eve", 99);

    printTable(ht);

    // Lookup Test
    int score;
    if (get(ht, "Charlie", &score)) {
        printf("\nLookup: Charlie's Score = %d\n", score);
    }

    freeTable(ht);
    return 0;
}

unsigned int hash(const char *key) {
    unsigned long hash = 5381;
    int c;

    while((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % TABLE_SIZE;
}


HashTable* createTable() {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    return table;
}

void insert(HashTable *table, const char *key, int value) {
    unsigned int index = hash(key);
    Node *current = table->buckets[index];

    // check if key already exists; update value if found
    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
}

int get(HashTable *table, const char *key, int *outValue) {
    unsigned int index = hash(key);
    Node *current = table->buckets[index];

    while(current) {
        if (strcmp(current->key, key) == 0) {
            *outValue = current->value;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int deleteKey(HashTable *table, const char *key) {
    unsigned int index = hash(key);
    Node *current = table->buckets[index];
    Node *prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                table->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->key);
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

void printTable(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Bucket [%d]: ", i);
        Node *curr = table->buckets[i];
        while (curr != NULL) {
            printf("(%s: %d) -> ", curr->key, curr->value);
            curr = curr->next;
        }
        printf("NULL\n");
    }
}

void freeTable(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *curr = table->buckets[i];
        while (curr != NULL) {
            Node *next = curr->next;
            free(curr->key);
            free(curr);
            curr = next;
        }
    }
    free(table);
}