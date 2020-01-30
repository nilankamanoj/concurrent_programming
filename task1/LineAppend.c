//compile : gcc -g -Wall -o LineAppend LineAppend.c -pthread
//run ./LineAppend

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>

void *Thread_Operation();
char final_out[15][20];

int main()
{
    key_t key = ftok("shmfile", 65); //generate unique key to share memory

    pthread_t *thread_handlers;
    thread_handlers = malloc(sizeof(pthread_t) * 3); //array of threads

    printf("Appending..\n");

    int i = 0;
    while (i < 3)
    {
        pthread_create(&thread_handlers[i], NULL, (void *)Thread_Operation, (void *)i); //creating threads from main threaad
        i++;
    }
    i = 0;
    while (i < 3)
    {
        pthread_join(thread_handlers[i], NULL); //join threads to main thread
        i++;
    }
    i = 0;
    char *result = malloc(301);
    while (i < 15)
    {
        printf("%s", final_out[i]);
        if (i == 0)
        {
            strcpy(result, final_out[i]);
        }
        else
        {
            strcat(result, final_out[i]);
        }
        i++;
    }

    printf("creating shared memory.\n");
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str = (char *)shmat(shmid, (void *)0, 0);

    printf("writing to shared memory.\n");
    strcpy(str, result);
    printf("done..\n");
    shmdt(str);
    return 0;
}

void *Thread_Operation(void *rank)
{
    long my_rank = (long)rank;
    FILE *file = fopen("data.txt", "r");
    char line[256];

    int i = 0;
    while (fgets(line, sizeof(line), file))
    {
        size_t n = strlen(line);
        if (n && line[n - 1] == '\n')
            line[n - 1] = '\0';

        if (my_rank == 0)
        {
            //I read names
            if (i < 5)
            {
                strcpy(final_out[i * 3], strcat(line, " from "));
            }
        }
        else if (my_rank == 1)
        {
            //I read cities
            if (i > 4 && i < 10)
            {
                strcpy(final_out[(i - 5) * 3 + 1], strcat(line, ". Age: "));
            }
        }
        else if (my_rank == 2)
        {
            //I read age
            if (i > 9)
            {
                line[n - 1] = '\n';
                strcpy(final_out[(i - 10) * 3 + 2], line);
            }
        }
        i++;
    }
    fclose(file);
    return 0;
}