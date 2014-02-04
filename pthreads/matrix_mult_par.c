#include "matrix_mult_par.h"
#include "matrix_mult_common.h"

/* Initialization */
int main (int argc, char** argv) {
	int j,k;
	
	srandom(SEED);
	char fname[50];
	
	sprintf(fname,"MM_PAR.%dP.%d.txt",NUM_THREADS,ARRAY_SIZE);

	f = fopen(fname,"w+");

	/*The number of threads */
	fprintf(f,"Number of threads = %d\n",NUM_THREADS);
	/*The number of data points for each experiment */
	for(j=0; j<NUM_DATA_POINTS; j++) {
		fprintf(f,"Array size = %d\n",ARRAY_SIZE);
		CHUNK_SIZE = ARRAY_SIZE/NUM_THREADS;
		fprintf(f,"Chunk size = %d\n",CHUNK_SIZE);
		
		/* The inner loop to average for each data point */
		for(k=0;k<NUM_TRIALS; k++) {
			fprintf(f,"Trial = %d\n",k);

			N = init_array();
			M = init_array();
			C = init_array();
			
			randomize_array(N);
			randomize_array(M);
			
			par_multiply();
			
			if(PRINT_SOLUTION)
				print_array(C);

			free(N);
			free(M);
			free(C);
			
		}
		ARRAY_SIZE += ARRAY_SIZE;
	}
	fclose(f);
	pthread_exit(NULL);
	return 0;
}

/*
	Set up the parallelization (init threads and barriers)
*/
void par_multiply() {
	int i;

	// Barrier initialization
	if(0!= pthread_barrier_init(&barrier1, NULL, NUM_THREADS))
	{
		fprintf(stderr,"Could not create a barrier\n");
		exit(-1);
	}
	
	if(0!= pthread_barrier_init(&barrier2, NULL, NUM_THREADS))
	{
		fprintf(stderr,"Could not create a barrier\n");
		exit(-1);
	}

	for(i=1; i<NUM_THREADS; i++) {
		TCB[i]._id = i;
		if(0!=pthread_create(&threads[i], NULL, multiply,(void*)&TCB[i])) {
			fprintf(stderr,"Pthread %d failed to execute, exiting...\n",i);
			exit(-1);
		}
	}

	TCB[0]._id = 0;

	multiply((void*)&(TCB[0]));

	// Barrier destruction
	if(0!=pthread_barrier_destroy(&barrier1))
	{
		fprintf(stderr,"Could not destroy a barrier\n");
		exit(-1);
	}
	// Barrier destruction
	if(0!=pthread_barrier_destroy(&barrier2))
	{
		fprintf(stderr,"Could not destroy a barrier\n");
		exit(-1);
	}
}

/*The parallel multiplication code.  Each thread acts on a chunk of C
	TO BE TUNED
 */
void * multiply(void * arg) {
	int i,j,k,t, this_id, rc;
	time_t t0, t1;
	clock_t c0, c1;

	ThreadControlBlock_T * this_TCB;
	
	this_TCB = (ThreadControlBlock_T *) arg;

	this_id = this_TCB->_id;
	
	//Sync before multiplying	
	rc=pthread_barrier_wait(&barrier1);
	if(rc!=0 && rc!=PTHREAD_BARRIER_SERIAL_THREAD) {
		fprintf(stderr,"Could not wait on a barrier, exiting...\n");
		exit(-1);
	}

	//Start Timing
	MASTER{
		t0 = time(NULL);
		c0 = clock();
	}
	
	for(i=(this_id*CHUNK_SIZE); i<((this_id+1)*CHUNK_SIZE); i++) {
		for(j=0; j<ARRAY_SIZE; j++) {
			t = 0;
			for(k=0; k<ARRAY_SIZE; k++) {
				t+=N[i][k]*M[k][j];
			}
			C[i][j] = t;
		}
	}

	//Sync before reporting
	rc=pthread_barrier_wait(&barrier2);
	if(rc!=0 && rc!=PTHREAD_BARRIER_SERIAL_THREAD) {
		fprintf(stderr,"Could not wait on a barrier, exiting...\n");
		exit(-1);
	}
	//Stop timing and report
	t1 = time(NULL);
	c1 = clock();
	MASTER {
		fprintf (f,"Elapsed wall clock time: %lds\n", (long) (t1 - t0));
		fprintf (f,"Elapsed CPU time: %f\n", (float) (c1 - c0)/CLOCKS_PER_SEC);
	}
}
