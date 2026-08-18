#include <stdio.h>
#include <string.h>
#include "student.h"

Student createStudent(int id, const char *name, float gpa) {
    Student s;
    s.id = id;
    strncpy(s.name, name, sizeof(s.name) - 1);
    s.name[sizeof(s.name) - 1] = '\0';
    s.gpa = gpa;
    return s;
}

void printStudent(const Student *s) {
    printf("Id: %d | name: %-10s | gpa: %.2f\n", s->id, s->name, s->gpa);
}