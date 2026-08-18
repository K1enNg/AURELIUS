#include <stdio.h>
#include <string.h>

//define a nested struct
typedef struct {
    char city[30];
    int zipCode;
} Address;

//define main struct
typedef struct {
    int id;
    char name[50];
    float gpa;
    Address location;
} Student;

void printStudentInfo(const Student *s);

int main() {

    Student st1 = {
        .id = 403,
        .name = "Ashley",
        .gpa = 3.2f,
        .location = {"Montreal", 101}
    };

    printf("student ID: %d\n", st1.id);
    printf("name: %s\n", st1.name);
    printf("GPA: %f\n", st1.gpa);
    printf("address: %s", st1.location.city);

    return 0;
}

void printStudentInfo(const Student *s){
    printf("ID: %d\n", s->id);
    printf("Name: %s\n", s->name);
    printf("GPA: %f\n", s->gpa);
    printf("Address: %s, %d\n", s->location.city, s->location.zipCode);
}