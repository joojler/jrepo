#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MALLOC_SIZE (1024*1024*3)

int shmid;
int shmkey = 1222;


void shared_memory_test (void)
{
	int shm_size = 1024*1024*4;

	shmkey += 1;
	shmid = shmget(shmkey, shm_size, IPC_CREAT | 0666);
  	// 1024 = your preferred size for share memory
  	// IPC_CREAT  its a flag to create shared memory
	printf("create shared memory %d bytets, id = %d\n", shm_size, shmid);
  	//now attach a memory to this share memory
  	char *shmpointer = shmat(shmid, NULL, 0);

  	//do your work with the shared memory 
	memset (shmpointer, shm_size, 0x11);
	//printf("shmem is written to %x\n", (unsigned int)shmpointer);
  	//read -write will be done with the *shmppointer
  	//after your work is done deattach the pointer

  	//shmdt(&shmpointer, NULL);
}


void *thread_func(void *p)
{
	static int thread_index = 0;

	printf("thread id: %d\n", thread_index);
	thread_index++;
	while (1);

	return NULL;
}

int thread_test(int thread_num)
{
	int i = 0;
	pthread_t my_thread;

	for (i=0; i<thread_num; i++) {
		pthread_create (&my_thread, NULL, thread_func, NULL);
	}
}


int main(int argc, char *argv[])
{
	char *p[1000];
	char input_c;
	int i = 0, j = 0;
	void *array;

	while (1) {
		p[i] = malloc(MALLOC_SIZE);
		if (p) {
			printf("malloc 3M done!\n");
		}

		input_c = getchar();
		switch (input_c) {
			case 'f':
				for (j = 0; j <= i; j++)
					free(p[j]);
				i = 0;
				printf("free all p\n");
				break;
			
			case 's':
				array = mmap (NULL, MALLOC_SIZE*5,PROT_NONE ,
						MAP_PRIVATE | MAP_ANON, -1, 0);
				if (array)
					printf("mmap 9M done!\n");
				i++;
				//memset(array, 0, MALLOC_SIZE*5);
				break;
			case 't':
				thread_test (100);
				printf("thread_test done\n");
				break;
			case 'm':
				shared_memory_test();
				break;
			case 'w':
				memset(p[i], 0, MALLOC_SIZE);
			default:
				i++;
				break;
		}
	}	

	return 0;
}
