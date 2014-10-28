#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//shm_open include
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h> 
//ftruncate include
#include <unistd.h>
#include <sys/types.h>

#define true 1


 
int main()
{
    key_t shmKey;
    int shmId;
    int size = 20;
    pid_t pid;
    char *word;

    // ftok take two params, and return a key_t.
    // key_t =  ftok(const char *pathname, int proj_id);
    if ((shmKey = ftok(".",'a')) == (key_t) - 1)
    {
        printf("key generate error\n");
        exit(1);
    }

    if ((shmId = shmget(shmKey, size, IPC_CREAT | 0600)) < 0)
    {
        printf("error create shared memory: %d\n", shmId);
        exit(1);
    }

    if ((word = (char*)shmat(shmId, NULL, 0)) == (char *) -1)
    {
        printf("shmat error\n");
        exit(1);
    }

    while(true)
    {
        printf("=> ");
        gets(word);
    }
}