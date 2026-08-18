#include <stdio.h>
#include "student.h"

int main() {
    Student alicia = createStudent(101, "Alice", 3.52);
    printStudent(&alicia);
    return 0;
}