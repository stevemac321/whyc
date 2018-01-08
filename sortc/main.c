#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int NUM_ELEMS = 1000000;
static inline int cmp(const void *x, const void *y)
{
	return *(int *)x - *(int *)y;
}
int main()
{
	int a[NUM_ELEMS];
        clock_t start, stop;

	srand(time(NULL));
	for (int i = 0; i < NUM_ELEMS; i++)
		a[i] = rand() % NUM_ELEMS - 100;

#pragma omp parallel
        start = clock();
	qsort(&a[0], NUM_ELEMS, sizeof(int), cmp);

	stop = clock();
        printf("%.4lf\n", (stop - start) / ((double) CLOCKS_PER_SEC));
}
