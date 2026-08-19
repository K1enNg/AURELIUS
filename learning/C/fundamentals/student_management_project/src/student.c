#include "student.h"

StudentDatabase* initDatabase(int intitialCapacity) {
    StudentDatabase *db = (StudentDatabase *)malloc(sizeof(StudentDatabase));
    if (db == NULL) return NULL;

    db->data = (Student *)malloc(intitialCapacity * sizeof(Student));
    if (db->data == NULL){
        free(db);
        return NULL;
    }

    db->count = 0;
    db->capacity = intitialCapacity;
    return db;
}

void freeDatabase(StudentDatabase *db) {
    if (db != NULL) {
        if (db->data != NULL) {
            free(db->data);
        }
        free(db);
    }
}

void addStudent(StudentDatabase *db) {
    if (db->count == db->capacity) {
        int newCapacity = db->capacity * 2;
        Student *temp = (Student *)realloc(db->data, newCapacity * sizeof(Student));
        if (temp == NULL) {
            printf("system failed to expand database memory\n");
            return;
        }
        db->data = temp;
        db->capacity = newCapacity;
        printf("system expanded to capacity: %d\n", db->capacity);
    }

    Student *st = &db->data[db->count];

    printf("\nEnter Student ID: ");
    scanf("%d", &st->id);

    printf("Enter Name (single word or no spaces): ");
    scanf("%49s", st->name);

    printf("Enter GPA: ");
    scanf("%f", &st->gpa);

    printf("Enter City: ");
    scanf("%29s", st->location.city);

    printf("Enter ZIP Code: ");
    scanf("%d", &st->location.zipCode);

    db->count++;
    printf("student record added.");
}

void displayStudents(const StudentDatabase *db) {
    if (db->count == 0) {
        printf("\nno student records in memory.\n");
    }

    printf("STUDENT RECORD\n");
    for (int i = 0; i < db->count; i++) {
        const Student *st = &db->data[i];
        printf("Id: %-5d | name: %-12s | gpa: %-4.2f | city: %-10s (%d)\n",
                st->id,     st->name,      st->gpa,     st->location.city,st->location.zipCode);                
    }
}

void searchStudentById(const StudentDatabase *db) {
    if (db->count == 0) {
        printf("no student record available");
        return;
    }

    int targetId;
    printf("\nEnter Student ID to search: ");
    scanf("%d", &targetId);

    for (int i = 0; i < db->count; i++) {
        if(db->data[i].id == targetId) {
            const Student *st = &db->data[i];
            const Student *s = &db->data[i];
            printf("\n--- Record Found ---\n");
            printf("ID:       %d\n", s->id);
            printf("name:     %s\n", s->name);
            printf("gpa:      %.2f\n", s->gpa);
            printf("city:     %s\n", s->location.city);
            printf("zip: %d\n", s->location.zipCode);
        }
    }

    printf("student not found.");
}

int loadStudentsFromFile(StudentDatabase *db, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    int loadedCount = 0;
    Student temp;

    while (fscanf(file, "%d %49s %f %29s %d",
                        &temp.id, temp.name, &temp.gpa,
                        temp.location.city, &temp.location.zipCode) == 5) {

        if (db->count == db->capacity) {
            int newCapacity = db->capacity*2;
            Student *reallocData = (Student *)realloc(db->data, newCapacity * sizeof(Student));
            if (reallocData == NULL) {
                fclose(file);
                return loadedCount;
            }
            db->data = reallocData;
            db->capacity = newCapacity;
        }

        db->data[db->count] = temp;
        db->count++;
        loadedCount++;
    }

    fclose(file);
    return loadedCount;
}

int saveStudentsToFile(const StudentDatabase *db, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("error saving record to file");
        return 0;
    }

    for (int i = 0; i < db->count; i++) {
        const Student *st = &db->data[i];
        fprintf(file, "%d %s %.2f %s %d\n",
                st->id, st->name, st->gpa, st->location.city, st->location.zipCode);

    }

    fclose(file);
    return 1;
}




