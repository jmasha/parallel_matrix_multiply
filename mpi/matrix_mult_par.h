/* A Header File */
#include <mpi.h>

//#define NUM_THREADS 4
//#define MASTER if (rank == 0)

int CHUNK_SIZE;

void par_multiply();
long *multiply(long* our_chunk);
