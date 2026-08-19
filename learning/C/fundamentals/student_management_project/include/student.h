#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char city[50];
    int zipCode;
} Address;

typedef struct {
    int id;
    char name[50];
    float gpa;
    Address location;
} Student;

typedef struct {
    Student *data;
    int count;
    int capacity;
} StudentDatabase;

StudentDatabase* initDatabase(int intialCapacity);
void freeDatabase(StudentDatabase *db);

void addStudent(StudentDatabase *db);
void displayStudents(const StudentDatabase *db);
void searchStudentById(const StudentDatabase *db);

int loadStudentsFromFile(StudentDatabase *db, const char *filename);
int saveStudentsToFile(const StudentDatabase *db, const char *filename);

#endif