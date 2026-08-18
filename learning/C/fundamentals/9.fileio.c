#include <stdio.h>
#include <stdlib.h>

// fopen(): opens or creates a file stream. Returns NULL if failed.
// fprintf(): writes formatted text into the file stream.
// fscanf(): reads formatted data from the file stream.
// fgetc(): reads single character from file. Returns EOF at file end.
// fgets(): reads an entire line of text safely up to size bytes.
// fclose(): closes the file stream and flushes buffers to disk.

int main() {
    FILE *file = NULL;

    printf("1. write to file\n");
    file = fopen("students.txt", "w");
    if (file == NULL) {
        perror("error opening file for writing");
        return 1;
    }

    fprintf(file, "%d %s %.2f\n", 101, "Alicia", 3.8);
    fprintf(file, "%d %s %.2f\n", 102, "Baron", 3.92);
    fprintf(file, "%d %s %.2f\n", 103, "Courtney", 3.76);

    fclose(file); //always close file
    printf("data succesfully saved.\n\n");

    printf("2. read from file\n");
    file = fopen("students.txt", "r");
    if (file == NULL) {
        perror("error opening file for reading");
        return 1;
    }

    int id;
    char name[50];
    float gpa;

    while(fscanf(file, "%d %s %f", &id, name, &gpa) == 3) {
        printf("ID: %d | name: %-10s | gpa: %.2f\n", id, name, gpa);
    }

    fclose(file);
    return 0;
}