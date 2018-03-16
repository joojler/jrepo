
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define THROUGHPUT      (1000*1024*1024)
#define MAX_THREAD 	(256)
#define BUF_SIZE	(16*1024)
#define SLEEP_INTERVAL	(10) //ms

static char *src, *dest;
static int bytes_per_memcpy = 0;

static void *start(void *p)
{
    int *idx = (int *)p;
    int i = 0;
    char *src, *dest;
    struct timeval current;
    unsigned long long before_t, after_t; 

    printf("thread %d entering\n", *idx);

    src = (char *)malloc(bytes_per_memcpy);
    if (!src)
        printf("malloc src failed!\n");
    memset(src, 0x11, bytes_per_memcpy);

    dest = (char *)malloc(bytes_per_memcpy);
    if (!dest)
        printf("malloc dest failed!\n");
   
    while (1) {
//      gettimeofday(&current, NULL);
//       before_t = current.tv_usec+current.tv_sec*1000000;

	memcpy(dest, src, bytes_per_memcpy);

//        gettimeofday(&current, NULL);
//        after_t = current.tv_usec+current.tv_sec*1000000;
//        printf("mempcy consume : %lu us\n", after_t - before_t);

        usleep(1000*SLEEP_INTERVAL);
    }
    
    return NULL;
}

int main(int argc, char *argv[])
{
    int i = 0;
    pthread_t thread_h;
    int ret = 0;
    int idx[1024];
    int thread_num = 0;

    if (argv[1])
        thread_num = atoi(argv[1]);
    else 
        thread_num = MAX_THREAD;

    bytes_per_memcpy = THROUGHPUT / thread_num / ( 1000 / SLEEP_INTERVAL);
    printf ("bytes_per_memcpy = %d\n", bytes_per_memcpy);  

    for (i = 0; i < thread_num; i++) {
        idx[i] = i;
        ret = pthread_create(&thread_h, NULL, start, &idx[i]);
        if (ret)
            printf("create thread %d failed, ret %d\n", i, ret);
    }
    
    while (1);    
    return 0;
}
