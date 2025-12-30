#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 100
#define MAX_ID_LEN 20

// Structure
typedef struct {
    char name[MAX_NAME_LEN];
    char id[MAX_ID_LEN];
    float marks[5];
    float percentage;
} Student;

// Function declarations
void addStudent();
void viewStudents();
void searchStudent();
int is_valid_name(const char *name);
int is_valid_id(const char *id);
void clear_input_buffer();

// Main menu
int main() {
    int choice;

    do {
        printf("\n==============================\n");
        printf(" UNIVERSITY RESULT MANAGEMENT\n");
        printf("==============================\n");
        printf("1. Add Student Result\n");
        printf("2. View All Results\n");
        printf("3. Search Result by ID\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 4);

    return 0;
}

// Clear input buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Validate name
int is_valid_name(const char *name) {
    int hasLetter = 0;
    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i]) && name[i] != ' ')
            return 0;
        if (isalpha(name[i]))
            hasLetter = 1;
    }
    return hasLetter;
}

// Validate ID
int is_valid_id(const char *id) {
    for (int i = 0; id[i]; i++) {
        if (!isalnum(id[i]))
            return 0;
    }
    return strlen(id) > 0;
}

// Add student
void addStudent() {
    Student s;
    FILE *fp = fopen("results.dat", "ab");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Name input
    do {
        printf("Enter Student Name: ");
        fgets(s.name, MAX_NAME_LEN, stdin);
        s.name[strcspn(s.name, "\n")] = '\0';

        if (!is_valid_name(s.name))
            printf("Invalid name! Only letters & spaces allowed.\n");
    } while (!is_valid_name(s.name));

    // ID input
    do {
        printf("Enter Student ID: ");
        scanf("%s", s.id);
        clear_input_buffer();

        if (!is_valid_id(s.id))
            printf("Invalid ID! Only alphanumeric allowed.\n");
    } while (!is_valid_id(s.id));

    // Marks input
    s.percentage = 0;
    for (int i = 0; i < 5; i++) {
        printf("Enter marks for subject %d: ", i + 1);
        scanf("%f", &s.marks[i]);
        s.percentage += s.marks[i];
    }
    clear_input_buffer();
    s.percentage /= 5;

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("Student record added successfully!\n");
}

// View all students
void viewStudents() {
    Student s;
    FILE *fp = fopen("results.dat", "rb");

    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\n----- Student Results -----\n");
    while (fread(&s, sizeof(Student), 1, fp)) {
        printf("\nName: %s", s.name);
        printf("\nID: %s", s.id);
        printf("\nPercentage: %.2f%%\n", s.percentage);
    }

    fclose(fp);
}

// Search student
void searchStudent() {
    Student s;
    char searchID[MAX_ID_LEN];
    int found = 0;

    FILE *fp = fopen("results.dat", "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("Enter Student ID to search: ");
    scanf("%s", searchID);
    clear_input_buffer();

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, searchID) == 0) {
            printf("\nStudent Found!");
            printf("\nName: %s", s.name);
            printf("\nID: %s", s.id);
            printf("\nPercentage: %.2f%%\n", s.percentage);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student record not found.\n");

    fclose(fp);
}
