//compile : gcc -g -Wall -o Actions Actions.c -pthread
//run ./Actions

/**
Reset data file to inital state by ctrl + a, ctrl + c on "data.txt.orig" 
and ctrl + v on "data.txt"
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex;

//HR functions
int *addEmployee();
int *deleteEmployee();
int *editEmployee();
int *editAttendance();
int *editSalaryPerDay();
int *viewTotalSalary();

//Accountant functions
int *viewEmployees();
int *viewAttendance();
int *viewSalaryPerDay();
int *calcTotalSalary();
int *editTotalSalary();

void *HR_Sample_Thread_Operation();
void *Accountant_Sample_Thread_Operation();

int main()
{

    pthread_t *thread_handlers;
    thread_handlers = malloc(sizeof(pthread_t) * 2);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread_handlers[0], NULL, (void *)HR_Sample_Thread_Operation, NULL);
    pthread_create(&thread_handlers[1], NULL, (void *)Accountant_Sample_Thread_Operation, NULL);

    int i = 0;
    while (i < 2)
    {
        pthread_join(thread_handlers[i], NULL);
        i++;
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}

void *HR_Sample_Thread_Operation()
{
    //can edit order, qty, arguments as required
    addEmployee("emp4", 10, 1300);
    deleteEmployee("emp2");
    addEmployee("emp5", 10, 1300);
    deleteEmployee("emp2");
    editEmployee("emp1", "emp8");
    editAttendance("emp4", 12);
    editSalaryPerDay("emp5", 1350);
    viewTotalSalary("emp8");
}

void *Accountant_Sample_Thread_Operation()
{
    //can edit order, qty, arguments as required
    viewEmployees();
    viewAttendance("emp4");
    viewAttendance("emp2");
    viewSalaryPerDay("emp5");
    calcTotalSalary("emp5");
    editTotalSalary("emp5", 45623);
}

int *addEmployee(char name[15], long attendance, long salaryPerDay)
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r+");
    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        strcpy(name_, strtok(line, ","));
        if (strcmp(name, name_) == 0)
        {
            printf("Name should be unique.\n");
            pthread_mutex_unlock(&mutex);
            return 0;
        }
    }
    char new_line[256];
    char attendance_[5];
    char salaryPerDay_[5];
    sprintf(attendance_, "%lu", attendance);
    sprintf(salaryPerDay_, "%lu", salaryPerDay);

    strcpy(new_line, name);
    strcat(new_line, ",");
    strcat(new_line, attendance_);
    strcat(new_line, ",");
    strcat(new_line, salaryPerDay_);
    strcat(new_line, ",");
    strcat(new_line, "0");
    strcat(new_line, "\n");
    fputs(new_line, file);
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int *deleteEmployee(char name[15])
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r+");
    char lines[300 * 256];
    char line[256];
    int deleted = 0;
    int i = 0;

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        char line_[256];
        strcpy(line_, line);
        strcpy(name_, strtok(line, ","));
        if (strcmp(name, name_) == 0)
        {
            printf("deleted employee: %s\n", name);
            deleted = 1;
        }
        else
        {
            if (i == 0)
                strcpy(lines, line_);
            else
            {
                strcat(lines, line_);
            }
        }
        i++;
    }
    if (deleted == 0)
    {
        printf("employee not available.\n");
    }
    fclose(file);
    file = fopen("data.txt", "w");
    fprintf(file, lines);
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int *editEmployee(char name[15], char newName[15])
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r+");
    char lines[300 * 256];
    char line[256];
    int edited = 0;
    int i = 0;

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        char line_[256];
        strcpy(line_, line);
        strcpy(name_, strtok(line, ","));
        if (strcmp(name, name_) == 0)
        {
            strcpy(line_, newName);
            strcat(line_, ",");
            strcat(line_, strtok(NULL, ","));
            strcat(line_, ",");
            strcat(line_, strtok(NULL, ","));
            strcat(line_, ",");
            strcat(line_, strtok(NULL, ","));

            printf("edited employee: %s\n", name);
            edited = 1;
        }

        if (i == 0)
            strcpy(lines, line_);
        if (i > 0)
        {
            strcat(lines, line_);
        }

        i++;
    }
    if (edited == 0)
    {
        printf("employee not available.\n");
    }
    fclose(file);
    file = fopen("data.txt", "w");
    fprintf(file, lines);
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int *editAttendance(char name[15], long attendance)
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r+");
    char lines[300 * 256];
    char line[256];
    int edited = 0;
    int i = 0;

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        char line_[256];
        strcpy(line_, line);
        strcpy(name_, strtok(line, ","));
        if (strcmp(name, name_) == 0)
        {
            char attendance_[5];
            sprintf(attendance_, "%lu", attendance);

            strcpy(line_, name);
            strcat(line_, ",");
            strcat(line_, attendance_);
            strtok(NULL, ",");
            strcat(line_, ",");
            strcat(line_, strtok(NULL, ","));
            strcat(line_, ",");
            strcat(line_, strtok(NULL, ","));

            printf("edited employee: %s 's attendance\n", name);
            edited = 1;
        }

        if (i == 0)
            strcpy(lines, line_);
        if (i > 0)
        {
            strcat(lines, line_);
        }

        i++;
    }
    if (edited == 0)
    {
        printf("employee not available.\n");
    }
    fclose(file);
    file = fopen("data.txt", "w");
    fprintf(file, lines);
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int *editSalaryPerDay(char name[15], long salaryPerDay)
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r+");
    char lines[300 * 256];
    char line[256];
    int edited = 0;
    int i = 0;

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        char line_[256];
        strcpy(line_, line);
        strcpy(name_, strtok(line, ","));
        if (strcmp(name, name_) == 0)
        {
            char salaryPerDay_[5];
            sprintf(salaryPerDay_, "%lu", salaryPerDay);

            strcpy(line_, name);
            strcat(line_, ",");
            strcat(line_, strtok(NULL, ","));
            strcat(line_, ",");
            strcat(line_, salaryPerDay_);
            strtok(NULL, ",");
            strcat(line_, ",");
            strcat(line_, strtok(NULL, ","));

            printf("edited employee: %s 's attendance\n", name);
            edited = 1;
        }

        if (i == 0)
            strcpy(lines, line_);
        if (i > 0)
        {
            strcat(lines, line_);
        }

        i++;
    }
    if (edited == 0)
    {
        printf("employee not available.\n");
    }
    fclose(file);
    file = fopen("data.txt", "w");
    fprintf(file, lines);
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int *viewTotalSalary(char name[15])
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r");
    char line[256];
    int viewed = 0;

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        strcpy(name_, strtok(line, ","));
        if (strcmp(name, name_) == 0)
        {
            strtok(NULL, ",");
            strtok(NULL, ",");

            printf("total salary of employee %s: %s\n", name, strtok(NULL, ","));
            viewed = 1;
        }
    }
    if (viewed == 0)
    {
        printf("employee not available.\n");
    }
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int *viewEmployees()
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r");
    char line[256];
    printf("employee list : \n");

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        strcpy(name_, strtok(line, ","));
        printf("%s\n", name_);
    }
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int *viewAttendance(char name[15])
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r");
    char line[256];
    int viewed = 0;

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        strcpy(name_, strtok(line, ","));
        if (strcmp(name, name_) == 0)
        {
            printf("attendance of employee %s: %s\n", name, strtok(NULL, ","));
            viewed = 1;
        }
    }
    if (viewed == 0)
    {
        printf("employee not available.\n");
    }
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int *viewSalaryPerDay(char name[15])
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r");
    char line[256];
    int viewed = 0;

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        strcpy(name_, strtok(line, ","));
        if (strcmp(name, name_) == 0)
        {
            strtok(NULL, ",");
            printf("salary per day of employee %s: %s\n", name, strtok(NULL, ","));
            viewed = 1;
        }
    }
    if (viewed == 0)
    {
        printf("employee not available.\n");
    }
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int *calcTotalSalary(char name[15])
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r+");
    char lines[300 * 256];
    char line[256];
    int edited = 0;
    int i = 0;

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        long attendance, salaryPerDay, totalSalary;
        char attendance_[5], salaryPerDay_[5], totalSalary_[6];

        char line_[256];
        strcpy(line_, line);
        strcpy(name_, strtok(line, ","));
        if (strcmp(name, name_) == 0)
        {

            strcpy(line_, name);
            strcat(line_, ",");
            attendance = atol(strtok(NULL, ","));
            printf("%lu\n", attendance);
            sprintf(attendance_, "%lu", attendance);
            strcat(line_, attendance_);
            strcat(line_, ",");
            salaryPerDay = atol(strtok(NULL, ","));
            sprintf(salaryPerDay_, "%lu", salaryPerDay);
            strcat(line_, salaryPerDay_);
            strcat(line_, ",");
            totalSalary = attendance * salaryPerDay;
            sprintf(totalSalary_, "%lu", totalSalary);
            strcat(line_, totalSalary_);

            printf("calculated employee: %s 's total salary\n", name);
            edited = 1;
        }

        if (i == 0)
            strcpy(lines, line_);
        if (i > 0)
        {
            strcat(lines, line_);
        }

        i++;
    }
    if (edited == 0)
    {
        printf("employee not available.\n");
    }
    fclose(file);
    file = fopen("data.txt", "w");
    fprintf(file, lines);
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int *editTotalSalary(char name[15], long totalSalary)
{
    pthread_mutex_lock(&mutex);
    FILE *file = fopen("data.txt", "r+");
    char lines[300 * 256];
    char line[256];
    int edited = 0;
    int i = 0;

    while (fgets(line, sizeof(line), file))
    {
        char name_[15];
        char line_[256];
        strcpy(line_, line);
        strcpy(name_, strtok(line, ","));
        if (strcmp(name, name_) == 0)
        {
            char totalSalary_[5];
            sprintf(totalSalary_, "%lu", totalSalary);

            strcpy(line_, name);
            strcat(line_, ",");
            strcat(line_, strtok(NULL, ","));
            strcat(line_, ",");
            strcat(line_, strtok(NULL, ","));
            strcat(line_, ",");
            strcat(line_, totalSalary_);

            printf("edited employee: %s 's attendance\n", name);
            edited = 1;
        }

        if (i == 0)
            strcpy(lines, line_);
        if (i > 0)
        {
            strcat(lines, line_);
        }

        i++;
    }
    if (edited == 0)
    {
        printf("employee not available.\n");
    }
    fclose(file);
    file = fopen("data.txt", "w");
    fprintf(file, lines);
    fclose(file);
    pthread_mutex_unlock(&mutex);
    return 0;
}
