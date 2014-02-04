#include "matrix_mult_seq.h"
#include "matrix_mult_common.h"


/* Initialize the matrix and run a sequential multiplication */
int main (int argc, char** argv) {
	int j,k;
	
	//srandom(time(NULL));
	srandom(SEED);

	char fname[50];
	sprintf(fname,"MM_SEQ.1P.%d.txt",ARRAY_SIZE);

	f = fopen(fname,"a");
	
	/*The number of data points for each experiment */
	for(j=0; j<NUM_DATA_POINTS; j++) {
		fprintf(f,"Array size = %d\n",ARRAY_SIZE);
		/* The inner loop to average for each data point */
		for(k=0;k<NUM_TRIALS; k++) {
			fprintf(f,"Trial = %d\n",k);

			N = init_array();
			M = init_array();
			C = init_array();
			
			randomize_array(N);
			randomize_array(M);
			
			multiply();
			if(PRINT_SOLUTION)
				print_array(C);
	
			free(N);
			free(M);
			free(C);
		}
		ARRAY_SIZE += ARRAY_SIZE;
	}
	return 0;
}


/*Multiply two nxn matrices */
void multiply() {
	int i, j, k, t;
	time_t t0, t1;
	clock_t c0, c1;


	//Start Timing
	t0 = time(NULL);
	c0 = clock();
	
	for(i=0; i<ARRAY_SIZE; i++) {
		for(j=0; j<ARRAY_SIZE; j++) {
			t = 0;
			for(k=0; k<ARRAY_SIZE; k++) {
				t+=N[i][k]*M[k][j];
			}
			C[i][j] = t;
		}
	}

	//Stop Timing
	t1 = time(NULL);
	c1 = clock();
	fprintf (f,"Elapsed wall clock time: %lds\n", (long) (t1 - t0));
	fprintf (f,"Elapsed CPU time:        %f\n", (float) (c1 - c0)/CLOCKS_PER_SEC);
}
