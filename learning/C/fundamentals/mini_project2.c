#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char city[30];
    int zipCode;
} Address;

typedef struct {
    int id;
    char name[50];
    float gpa;
    Address location;
} Student;

void addStudent(Student **students, int *count, int *capacity);
void displayStudent(const Student *student, int count);
void searchStudentbyId(const Student *students, int count);
void freeStudent(Student **students, int *count, int *capacity);

int main() {
    int capacity = 2;
    int count = 0;

    //allocate intial memory for dynamic array of Students
    Student *students = (Student *)malloc(capacity * sizeof(Student));
    if (students == NULL) {
        return 1;
    }

    int opt;
    while (opt != 4) {
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Exit\n");
        printf("Enter choice (1-4): ");
        scanf("%d", &opt);

        switch(opt) {
            case 1:
                addStudent(&students, &count, &capacity);
                break;
            case 2:
                displayStudent(students, count);
                break;
            case 3:
                searchStudentbyId(students, count);
                break;
            case 4:
                freeStudent(&students, &count, &capacity);
                break;
            default:
                printf("invalid option.\n");
        }
    }
    return 0;
}

void addStudent(Student **students, int *count, int *capacity) {
    if(*count == *capacity) {
        *capacity *= 2; //double capacity;
        Student *temp = (Student *)realloc(*students, (*capacity) * sizeof(Student));
        if (temp == NULL) {
            printf("failed to expand database memory.\n");
            return;
        }
        *students = temp;
        printf("[System] memory reallocated. New capacity: %d\n", *capacity);
    }

    Student *newSt = &((*students)[*count]);

    printf("\nEnter Student ID: ");
    scanf("%d", &newSt->id);

    printf("Enter Name (single word or no spaces): ");
    scanf("%49s", newSt->name);

    printf("Enter GPA: ");
    scanf("%f", &newSt->gpa);

    printf("Enter City: ");
    scanf("%29s", newSt->location.city);

    printf("Enter ZIP Code: ");
    scanf("%d", &newSt->location.zipCode);
    
    (*count)++;
}

void displayStudent(const Student *students, int count) {
    if (count == 0) {
        printf("\nNo student records available.\n");
        return;
    }

    printf("\n--- STUDENT RECORDS (%d) ---\n", count);
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Name: %-12s | GPA: %.2f | Location: %s (%d)\n",
               students[i].id,
               students[i].name,
               students[i].gpa,
               students[i].location.city,
               students[i].location.zipCode);
    }
}

void searchStudentbyId(const Student *students, int count) {
    if(count == 0) {
        printf("No student records available.\n");
        return;
    }

    int targetId;
    printf("\nEnter Student ID to search: ");
    scanf("%d", &targetId);

    for(int i = 0; i < count; i++) {
        if(students[i].id == targetId) {
            printf("\n--- Record Found ---\n");
            printf("ID:       %d\n", students[i].id);
            printf("Name:     %s\n", students[i].name);
            printf("GPA:      %.2f\n", students[i].gpa);
            printf("City:     %s\n", students[i].location.city);
            printf("ZIP Code: %d\n", students[i].location.zipCode);
            return;
        }
    }
    printf("Student with ID %d not found\n", targetId);
}

void freeStudent(Student **students, int *count, int *capacity) {
    if (*students != NULL) {
        free(*students);
        *students = NULL;
    }
    *count = 0;
    *capacity = 0;

    printf("memory free sucessfully.");
}