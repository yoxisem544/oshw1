#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//posix api
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
#define BUF 20
#define isEqual 0
#define notExist -1

typedef struct 
{
	char content[50];
	int times;
	int length;
} storedStrings;

typedef struct 
{
	storedStrings * item;
	int length;
	int matchIndex; // match matchIndex
	int index;//index of stored string counts
} Memory;

void initMemory(Memory *memory)
{
	memory->length = 20;
	memory->item = (storedStrings*)malloc(BUF * sizeof(storedStrings));
	strcpy(memory->item[0].content, "");
	memory->matchIndex = 0;
	memory->index = 0;
}

void extendMemory(Memory * memory)
{
	// memory->item = (storedStrings*)realloc(memory->item, sizeof(storedStrings) * (memory->length + 1));
	// memory->length += 1;	
}

int ifExist(char * inputString, Memory * memory)
{
	int i;
	for (i = 0; i < memory->length; i++)
	{
		// if match
		printf("%s%d\n", "(strcmp(memory->item[i].content, inputString))", (strcmp(memory->item[i].content, inputString)));
		if((strcmp(memory->item[i].content, inputString)) == isEqual)
		{
			memory->matchIndex = i;
			printf("%s\n", "isEqual");
			return isEqual;
		}
	}
	memory->matchIndex = 0;
	return notExist;
}

void check(Memory * memory, char * inputString)
{
	int ex = ifExist(inputString, memory);
	printf("%s\n", inputString);
	if(ex != notExist)
	{
		printf("%s\n", "Yes, it matched!");
		memory->item[memory->matchIndex].times += 1;
	}
	else
	{
		printf("%s\n", "No, not matched!");
		strcpy(memory->item[memory->index].content, inputString);
		memory->index += 1;
		printf("%s%d\n","memory->index = ", memory->index);
		printf("%s%s\n","memory->item[memory->index].content = ", memory->item[memory->index].content);
	}
}



int main()
{
	key_t shmKey;
	int shmId;
	int size = 20;
	pid_t pid;
	char *word;
	int i;

	// ftok take two params, and return a key_t.
	// key_t =  ftok(const char *pathname, int proj_id);
	// alloc shared memory, do the same thing before client started.
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

	//initial
	Memory memory;
	initMemory(&memory);
	printf("%d\n", memory.length);
	//extendMemory(&memory);

	//server started here!
	printf("%s\n", "Server started!");
	while(true)
	{
		if(strlen(word) != 0)
		{
			int counter = 0;
    		char *spl = strtok(word, " ");
     		
     		//string splitt here
	    	while (spl != NULL) 
	    	{
				if(strlen(spl) != 0)
				{
					printf("%s\n", spl);
					check(&memory,spl);

				}
		        counter += 1;
		        spl = strtok(NULL, " ");
		    }
		    //printf("%d\n", counter);
			//printf("%s\n", word);
			word[0] = '\0';// set to empty string to null while finished

			for (i = 0; i < memory.length; i++)
			{
				printf("%d%s%s\n", i+1," string is = ",memory.item[i].content);
			}
		}
	}
}