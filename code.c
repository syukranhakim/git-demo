#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Employee {
    char name[40];
    int age;
    float salary;
};

void addEmployee(FILE *fp);
void listEmployees(FILE *fp);
void modifyEmployee(FILE *fp);
void deleteEmployee(FILE *fp);

int main() {
    FILE *fp;
    char choice;

    fp = fopen("employees.dat", "rb+");
    if (fp == NULL) {
        fp = fopen("employees.dat", "wb+");
        if (fp == NULL) {
            printf("Cannot open file\n");
            exit(1);
        }
    }

    while (1) {
        printf("\nEmployee Management System\n");
        printf("1. Add Employee\n");
        printf("2. List Employees\n");
        printf("3. Modify Employee\n");
        printf("4. Delete Employee\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        fflush(stdin);
        choice = getchar();

        switch (choice) {
            case '1':
                addEmployee(fp);
                break;
            case '2':
                listEmployees(fp);
                break;
            case '3':
                modifyEmployee(fp);
                break;
            case '4':
                deleteEmployee(fp);
                break;
            case '5':
                fclose(fp);
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}

void addEmployee(FILE *fp) {
    struct Employee e;
    fseek(fp, 0, SEEK_END);
    printf("Enter name: ");
    scanf("%s", e.name);
    printf("Enter age: ");
    scanf("%d", &e.age);
    printf("Enter salary: ");
    scanf("%f", &e.salary);
    fwrite(&e, sizeof(e), 1, fp);
}

void listEmployees(FILE *fp) {
    struct Employee e;
    rewind(fp);
    while (fread(&e, sizeof(e), 1, fp) == 1) {
        printf("\nName: %s\nAge: %d\nSalary: %.2f\n", e.name, e.age, e.salary);
    }
}

void modifyEmployee(FILE *fp) {
    struct Employee e;
    char name[40];
    int found = 0;

    printf("Enter the name of the employee to modify: ");
    scanf("%s", name);
    rewind(fp);

    while (fread(&e, sizeof(e), 1, fp) == 1) {
        if (strcmp(e.name, name) == 0) {
            printf("Enter new age: ");
            scanf("%d", &e.age);
            printf("Enter new salary: ");
            scanf("%f", &e.salary);
            fseek(fp, -sizeof(e), SEEK_CUR);
            fwrite(&e, sizeof(e), 1, fp);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Employee not found\n");
    }
}

void deleteEmployee(FILE *fp) {
    struct Employee e;
    FILE *ft;
    char name[40];
    int found = 0;

    printf("Enter the name of the employee to delete: ");
    scanf("%s", name);
    ft = fopen("temp.dat", "wb");
    rewind(fp);

    while (fread(&e, sizeof(e), 1, fp) == 1) {
        if (strcmp(e.name, name) != 0) {
            fwrite(&e, sizeof(e), 1, ft);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(ft);

    remove("employees.dat");
    rename("temp.dat", "employees.dat");

    if (found) {
        printf("Employee deleted successfully\n");
    } else {
        printf("Employee not found\n");
    }

    fp = fopen("employees.dat", "rb+");
}
