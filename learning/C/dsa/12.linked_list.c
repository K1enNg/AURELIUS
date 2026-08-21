#include <stdio.h>
#include <stdlib.h>

// A Singly Linked List is a linear data structure composed of nodes where each node contains data and a pointer (next) to the next node in sequence. 
// Unlike arrays, linked lists do not store elements in contiguous memory locations—nodes are dynamically allocated on the heap wherever memory is available.

typedef struct Node{
    int data;
    struct Node *next;
} Node;

Node* createNode(int value);
void insertAtHead(Node **head, int value);
void insertAtTail(Node **head, int value);
void printList(const Node *head);
void freeList (Node **head);
Node* searchList(Node *head, int target);
void deleteByValue (Node **head, int target);
void reverseList (Node **head);

int main() {
    Node *head = NULL;

    insertAtHead(&head, 20);
    insertAtHead(&head, 10);
    printf("after inserting 10, 20 at head: \n");
    printList(head);

    insertAtTail(&head, 30);
    insertAtTail(&head, 40);
    printf("\nafter inserting 30, 40 at tail: \n");
    printList(head);

    reverseList(&head);
    printf("\nreverse list: ");
    printList(head);

    Node *found = searchList(head, 30);
    if (found) { printf("\nfound 30 at: %p\n", (void*)found); }

    found = searchList(head, 99);
    if (found) { printf("\nfound 99 at: %p\n", (void*)found); }
    else printf("99 not found.");

    deleteByValue(&head, 10);
    printList(head);

    freeList(&head);
    printf("\nafter freeing memory: \n");
    printList(head);

    return 0;
}


// 1. node creation helper
Node* createNode(int value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("heap memory allocation failed.");
        return NULL;
    }
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

// 2. insert at head (O(1) time)
void insertAtHead(Node **head, int value) {
    Node *newNode = createNode(value);
    if (newNode == NULL) return;

    newNode->next = *head; // point new node to current head
    *head = newNode;      // update pointer to point to new node
}

// 3. intert at tail (O(n) time)
void insertAtTail(Node **head, int value) {
    Node *newNode = createNode(value);
    if (newNode == NULL) return;

    // if list is empty, new node becomes head
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    // traverse to last node
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

// 4. traversal and printing (O(n) time)
void printList(const Node *head) {
    if (head == NULL) {
        printf("list is empty");
        return;
    }

    const Node *current = head;
    while (current != NULL) {
        printf("[%d] -> ", current->data);
        current = current->next;
    }
}

// 5. memory cleanup (O(n) time)
void freeList (Node **head) {
    Node *current = *head;
    Node *nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next; // store reference to next node before freeing
        free(current);
        current = nextNode;
    }

    *head = NULL; //reset caller's head pointer to NULL
} 

// 6. search for a key in the list
Node* searchList(Node *head, int target) {
    Node *current = head;
    while (current != NULL) {
        if (current->data == target) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 7. delete first node matchign target value
void deleteByValue (Node **head, int target) {
    if (*head == NULL) {
        printf("list is empty.");
    }
    Node *current = *head;
    Node *prev = NULL;
}

// 8. reverse
void reverseList (Node **head) {
    Node *prev = NULL;
    Node *current = *head;
    Node *next = NULL;

    while (current != NULL) {
        next = current->next; // save next
        current->next = prev; // reverse pointer
        prev = current;       // advance prev
        current = next;       // advance current
    }

    *head = prev;
}
