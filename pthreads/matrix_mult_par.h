/* A Header File */
#include <pthread.h>

#define NUM_THREADS 2
#define MASTER if (this_id == 0)

typedef struct ThreadControlBlock {
	int _id;
}ThreadControlBlock_T;

int CHUNK_SIZE;

ThreadControlBlock_T TCB[NUM_THREADS];

pthread_t threads[NUM_THREADS];
pthread_barrier_t barrier1;
pthread_barrier_t barrier2;

void par_multiply();
void * multiply(void * arg);
