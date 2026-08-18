#include <stdio.h>

float calculateAverage(int total, int count);
void printStatus(float average);

int main() {
    int scoreCount = 0;
    int currentScore = 0;
    int sum = 0;
    char opt = 'y';

    printf("============================\n");
    printf("   STUDENT GRADE ANALYZER   \n");
    printf("============================\n\n");

    while(opt == 'y' || opt == 'Y') {
        printf("input score #%d (0-100): ", scoreCount++);
        scanf("%d", &currentScore);

        if (currentScore < 0 || currentScore > 100) {
            printf("invalid score. (between 0-100)");
            continue;
        }

        sum += currentScore;
        scoreCount;

        printf("add another score? (y/n): ");
        scanf(" %c", &opt);
        printf("\n");
    }

    if (scoreCount > 0) {
        float avg = calculateAverage(sum, scoreCount);

        printf("Total Scores Entered: %d\n", scoreCount);
        printf("Sum of Scores:        %d\n", sum);
        printf("Class Average:        %.2f%%\n", avg);

        printStatus(avg);
    } else {
        printf("no valid scores entered. \n");
    }
    return 0;
}

float calculateAverage(int total, int count) {
    return (float)total/count;
}

void printStatus(float average) {
    if (average >= 90) {
        printf("EXELLENT\n");
    } else if (average >= 75) {
        printf ("GOOD\n");
    } else if (average >= 60) {
        printf ("PASSING GRADE");
    } else {
        printf("FAIL");
    }
}