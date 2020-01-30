//compile: gcc -g -Wall -o LineRead LineRead.c
//run: ./LineRead

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main()
{

    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    printf("reading shared memory content:\n");
    char *str = (char *)shmat(shmid, (void *)0, 0);
    printf("printing... \n%s", str);
    shmdt(str);
    printf("done..\n");
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}