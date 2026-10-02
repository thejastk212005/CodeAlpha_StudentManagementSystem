
#include <stdio.h>
#include <string.h>
#define FILE_NAME "students.dat"
#define TEMP_FILE "temp.dat"
struct Student
{
    int id;
    char name[50];
    int age;
    float marks;
};
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);
int main(void)
{
    int choice;
    do
    {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            return 1;
        }
        switch (choice)
        {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 6);
    return 0;
}
void addStudent(void)
{
    struct Student s, existing;
    FILE *fp;
    int found = 0;
    fp = fopen(FILE_NAME, "ab+");
    if (fp == NULL)
    {
        printf("Error opening student file!\n");
        return;
    }
    printf("Enter Student ID: ");
    if (scanf("%d", &s.id) != 1)
    {
        printf("Invalid ID!\n");
        fclose(fp);
        return;
    }
    rewind(fp);
    while (fread(&existing, sizeof(existing), 1, fp) == 1)
    {
        if (existing.id == s.id)
        {
            found = 1;
            break;
        }
    }
    if (found)
    {
        printf("Student ID already exists!\n");
        fclose(fp);
        return;
    }
    printf("Enter Student Name: ");
    scanf(" %49[^\n]", s.name);
    printf("Enter Student Age: ");
    if (scanf("%d", &s.age) != 1 || s.age <= 0)
    {
        printf("Invalid age!\n");
        fclose(fp);
        return;
    }
    printf("Enter Student Marks: ");
    if (scanf("%f", &s.marks) != 1 ||
        s.marks < 0 || s.marks > 100)
    {
        printf("Marks must be between 0 and 100!\n");
        fclose(fp);
        return;
    }
    fseek(fp, 0, SEEK_END);
    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);
    printf("Student added successfully!\n");
}
void displayStudents(void)
{
    struct Student s;
    FILE *fp = fopen(FILE_NAME, "rb");
    int found = 0;
    if (fp == NULL)
    {
        printf("No student records found.\n");
        return;
    }
    printf("\nID\tName\t\tAge\tMarks\n");
    printf("---------------------------------------------\n");
    while (fread(&s, sizeof(s), 1, fp) == 1)
    {
        printf("%d\t%-16s\t%d\t%.2f\n",
               s.id, s.name, s.age, s.marks);
        found = 1;
    }
    if (!found)
        printf("No student records found.\n");
    fclose(fp);
}
void searchStudent(void)
{
    struct Student s;
    FILE *fp = fopen(FILE_NAME, "rb");
    int id, found = 0;
    if (fp == NULL)
    {
        printf("No student records found.\n");
        return;
    }
    printf("Enter Student ID to search: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Invalid ID!\n");
        fclose(fp);
        return;
    }
    while (fread(&s, sizeof(s), 1, fp) == 1)
    {
        if (s.id == id)
        {
            printf("\nStudent Found!\n");
            printf("ID: %d\n", s.id);
            printf("Name: %s\n", s.name);
            printf("Age: %d\n", s.age);
            printf("Marks: %.2f\n", s.marks);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Student not found.\n");
    fclose(fp);
}
void updateStudent(void)
{
    struct Student s;
    FILE *fp = fopen(FILE_NAME, "rb+");
    int id, found = 0;
    long position;
    if (fp == NULL)
    {
        printf("No student records found.\n");
        return;
    }
    printf("Enter Student ID to update: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Invalid ID!\n");
        fclose(fp);
        return;
    }
    while (1)
    {
        position = ftell(fp);
        if (fread(&s, sizeof(s), 1, fp) != 1)
            break;
        if (s.id == id)
        {
            printf("Enter New Name: ");
            scanf(" %49[^\n]", s.name);
            printf("Enter New Age: ");
            if (scanf("%d", &s.age) != 1 || s.age <= 0)
            {
                printf("Invalid age!\n");
                fclose(fp);
                return;
            }
            printf("Enter New Marks: ");
            if (scanf("%f", &s.marks) != 1 ||
                s.marks < 0 || s.marks > 100)
            {
                printf("Marks must be between 0 and 100!\n");
                fclose(fp);
                return;
            }
            fseek(fp, position, SEEK_SET);
            fwrite(&s, sizeof(s), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (found)
        printf("Student updated successfully!\n");
    else
        printf("Student not found.\n");
}
void deleteStudent(void)
{
    struct Student s;
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *temp;
    int id, found = 0;
    if (fp == NULL)
    {
        printf("No student records found.\n");
        return;
    }
    printf("Enter Student ID to delete: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Invalid ID!\n");
        fclose(fp);
        return;
    }
    temp = fopen(TEMP_FILE, "wb");
    if (temp == NULL)
    {
        printf("Error creating temporary file!\n");
        fclose(fp);
        return;
    }
    while (fread(&s, sizeof(s), 1, fp) == 1)
    {
        if (s.id == id)
        {
            found = 1;
        }
        else
        {
            fwrite(&s, sizeof(s), 1, temp);
        }
    }
    fclose(fp);
    fclose(temp);
    if (found)
    {
        if (remove(FILE_NAME) == 0 &&
            rename(TEMP_FILE, FILE_NAME) == 0)
        {
            printf("Student deleted successfully!\n");
        }
        else
        {
            printf("Error replacing student file!\n");
        }
    }
    else
    {
        remove(TEMP_FILE);
        printf("Student not found.\n");
    }
}
