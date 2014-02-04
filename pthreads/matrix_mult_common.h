#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEED time(NULL)
#define PRINT_SOLUTION 0

int NUM_DATA_POINTS = 7;
int NUM_TRIALS = 7;
int ARRAY_SIZE = 64;
int MAX_VALUE = 1024;

long **N;
long **M;
long **C;

FILE* f;

static long **init_array() {
	long **M;
	int i;
	if(!(M = (long **)malloc(ARRAY_SIZE*sizeof(long*)))) {
		fprintf(stderr, "Malloc failed, insf memory");
		exit(EXIT_FAILURE);
	}
	for(i=0; i<ARRAY_SIZE; i++) {
		if(!(M[i] = (long*)malloc(ARRAY_SIZE*sizeof(long)))) {
			fprintf(stderr, "Malloc failed, insf memory");
			exit(EXIT_FAILURE);
		}
	}
	return M;
};

static void randomize_array(long **M) {
	int i, j;
	for(i=0; i<ARRAY_SIZE; i++) {
		for(j=0; j<ARRAY_SIZE; j++) {
			M[i][j] = random() %MAX_VALUE +1;
		}
	}
};

static void print_array(long **M) {
	int i, j;
	for(i=0; i<ARRAY_SIZE; i++) {
		fprintf(stdout, "|");
		for(j=0; j<ARRAY_SIZE; j++) {
			fprintf(stdout,"%ld,",M[i][j]);
		}
		fprintf(stdout,"|\n");
	}
	fprintf(stdout,"\n");
};

