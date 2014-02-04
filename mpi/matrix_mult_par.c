#include "matrix_mult_par.h"
#include "matrix_mult_common.h"
#include <string.h>

/* Initialization */
int main (int argc, char** argv) {
	int trial, data_point, i, rank, size, index;
	time_t t0, t1;
	clock_t c0, c1;
		
	MPI_Status status;

	MPI_Datatype chunk;
	MPI_Datatype matrix;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	srandom(SEED);
	char fname[255];
	if(rank == 0) {
		
		sprintf(fname,"%sMM_MPI.%dP.%d.txt",LOG_PATH,size,ARRAY_SIZE);
		
		f = fopen(fname,"a");
		
		/*The number of threads */
		fprintf(f,"Number of threads = %d\n",size);
		fflush(f);
	}
	
	/*The number of data points for each experiment */
	for(data_point=0; data_point<NUM_DATA_POINTS; data_point++) {
		int square = ARRAY_SIZE*ARRAY_SIZE;

		CHUNK_SIZE = square/size;
		if(rank == 0) {
			fprintf(f,"Array size = %d\n",ARRAY_SIZE);
			fflush(f);
			fprintf(f,"Chunk size = %d\n",CHUNK_SIZE);
			fflush(f);
		}

		long *our_chunk;
		long *results;
		
		//Allocate the memory we are going to need for the data point
		if(!(our_chunk = (long *)malloc(CHUNK_SIZE*sizeof(long)))) {
			fprintf(stderr, "Our Chunk Malloc failed, insf memory\n");
			exit(EXIT_FAILURE);
		}
		
		if(!(results = (long *)malloc(CHUNK_SIZE*sizeof(long)))) {
			fprintf(stderr, "Results Malloc failed, insf memory\n");
			exit(EXIT_FAILURE);
		}

		MPI_Type_contiguous(CHUNK_SIZE, MPI_LONG, &chunk);
		MPI_Type_commit(&chunk);
		MPI_Type_contiguous(square, MPI_LONG, &matrix);
		MPI_Type_commit(&matrix);

		if(rank ==0) {
			N = init_array();
			C = init_array();
		}
		M = init_array();
		
		/* The inner loop to average for each data point */
		for(trial=0;trial<NUM_TRIALS; trial++) {		

			if(rank == 0) {
				fprintf(f,"Trial = %d\n",trial);
				fflush(f);
				
				randomize_array(N);
				randomize_array(M);

				/*Start Timing*/
				t0 = time(NULL);
				c0 = clock();

				//Send the data to the recievers
				for(i=1; i<size; i++) {
					index = i*ARRAY_SIZE;
					MPI_Send(&N[index], 1, chunk, i, 1, MPI_COMM_WORLD);
					MPI_Send(&M[0], 1, matrix, i, 1, MPI_COMM_WORLD);
				}
				//Set our chunk to the head
				our_chunk = &N[0];
			}

			//Receive our chunk and M
			if(rank != 0) {
				MPI_Recv(our_chunk, CHUNK_SIZE, MPI_LONG, 0, 1 ,MPI_COMM_WORLD, &status);
				MPI_Recv(M, square, MPI_LONG, 0, 1 ,MPI_COMM_WORLD, &status);
			}

			//Multiply our chunk and the matrix
			results = multiply(our_chunk);

			//Send our results back to the master
			if(rank != 0) {
				MPI_Send(&results[0], 1, chunk, 0, 1, MPI_COMM_WORLD);
			} else {
				memcpy(&C[0], results, CHUNK_SIZE*sizeof(long*));
			}

			//Gather the results from the slaves
			if(rank == 0) {
				for(i=1; i<size; i++) {
					MPI_Recv(&C[i*CHUNK_SIZE],CHUNK_SIZE, MPI_LONG, i, 1, MPI_COMM_WORLD, &status);
				}
			}

			MPI_Barrier(MPI_COMM_WORLD);
			//Stop timing and report
			if(rank == 0) {
				t1 = time(NULL);
				c1 = clock();
				fprintf (f,"Elapsed wall clock time: %lds\n", (long) (t1 - t0));
				fprintf (f,"Elapsed CPU time: %f\n", (float) (c1 - c0)/CLOCKS_PER_SEC);
				fflush(f);
				if(PRINT_SOLUTION) {
					print_array(N);
					print_array(M);
					print_array(C);
				}
			} 
			//End of Trial
		}
		
		//End of Data Point Free up the matrices and re-init
		if(rank == 0) {
			fprintf(f,"-------------------------------\n\n");
			fflush(f);
		}

		if(rank ==0) {
			free(N);
			free(C);
		}
		free(M);
		free(results);

		if(rank !=0) {
			free(our_chunk);
		}

		ARRAY_SIZE += ARRAY_SIZE;
	}

	//End of experiment, close the file pointer and leave
	if(rank == 0) {
		fclose(f);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Type_free(&chunk);
	MPI_Type_free(&matrix);
	MPI_Finalize();
	
	return 0;
}

/*The parallel multiplication code.  Each thread acts on a chunk of C*/

long* multiply(long* our_chunk) {
	int i,j,k, index, row_index, col_index;
	long sum;
	long *results;

	if(!(results = (long *)malloc(CHUNK_SIZE*sizeof(long)))) {
		fprintf(stderr, "Multiply Results Malloc failed, insf memory\n");
		exit(EXIT_FAILURE);
	}	
	
	//Multiply and return the appropriate block of C
	for(i=0; i<CHUNK_SIZE/ARRAY_SIZE; i++) {
		for(j=0; j<ARRAY_SIZE; j++) {
			sum = 0;
			index = (i*ARRAY_SIZE) + j;
			for(k=0; k<ARRAY_SIZE; k++) {
				row_index = (i*ARRAY_SIZE)+k;
				col_index = j+(k*ARRAY_SIZE);
				sum+=our_chunk[row_index]*M[col_index];
			}
			results[index] = sum;
		}
	}
	
	return results;
}
