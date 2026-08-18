#ifndef STUDENT_H
#define STUDENT_H

typedef struct {
    int id;
    char name[50];
    float gpa;
} Student;

void printStudent(const Student *s);
Student createStudent(int id, const char *name, float gpa);

#endif