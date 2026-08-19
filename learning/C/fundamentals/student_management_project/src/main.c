#include <stdio.h>
#include "student.h"

#define FILENAME "students.txt"

int main() {

    StudentDatabase *db = initDatabase(2);
    if (db == NULL) {
        printf("memory initialization failed.");
        return 1;
    }

    int loaded = loadStudentsFromFile(db, FILENAME);
    if (loaded > 0) {
        printf("loaded %d existing student records from %s\n", loaded, FILENAME);
    } else {
        printf("no exsisting database found.");

    }

    int opt = 0;
    while (opt != 4) {
        printf("\n=== STUDENT DATABASE SYSTEM ===\n");
        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Save & Exit\n");
        printf("Enter option (1-4): ");

        if (scanf("%d", &opt) != 1) {
            printf("invalid input.");
            break;
        } 

        switch(opt) {
            case 1:
                addStudent(db);
                break;
            case 2:
                displayStudents(db);
                break;
            case 3:
                searchStudentById(db);
                break;
            case 4:
                if (saveStudentsToFile(db, FILENAME)) {
                    printf("successfully saved to record.\n");
                } else {
                    printf("failed to save data.\n");
                }
                break;
            default:
                printf("invalid option. try again\n");
        }
    }

    freeDatabase(db);
    printf("memory cleaned up successfully.\n");

    return 0;
}