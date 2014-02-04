#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEED time(NULL)
#define PRINT_SOLUTION 0

int NUM_DATA_POINTS = 5;
int NUM_TRIALS = 7;
int ARRAY_SIZE = 128;
int MAX_VALUE = 1024;

long *N;
long *M;
long *C;

FILE* f;

char *LOG_PATH = ".";

static long *init_array() {
	long *M;
	if(!(M = (long *)malloc(ARRAY_SIZE*ARRAY_SIZE*sizeof(long)))) {
		fprintf(stderr, "Init Array Malloc failed, insf memory\n");
		exit(EXIT_FAILURE);
	}
	return M;
};

static void randomize_array(long *M) {
	int i, j, index;
	for(i=0; i<ARRAY_SIZE; i++) {
		for(j=0; j<ARRAY_SIZE; j++) {
			index = (i*ARRAY_SIZE) + j;
			M[index] = random() %MAX_VALUE +1;
		}
	}
};

static void print_array(long *M) {
	int i, j, index;
	for(i=0; i<ARRAY_SIZE; i++) {
		fprintf(stdout, "|");
		for(j=0; j<ARRAY_SIZE; j++) {
			index = (i*ARRAY_SIZE) + j;
			fprintf(stdout,"%ld,",M[index]);
		}
		fprintf(stdout,"|\n");
	}
	fprintf(stdout,"\n");
};

static void print_row(long *r) {
	int i;
	fprintf(stdout, "|");
	for(i=0; i<ARRAY_SIZE; i++) {
		fprintf(stdout,"%ld,",r[i]);
	}
	fprintf(stdout,"|\n\n");
};
