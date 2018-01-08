#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int NUM_ELEMS = 1000000;

static inline int compare(const void *x, const void *y)
{
	return *(int *)x - *(int *)y;
}
static inline void partition(double *v, int lower, int upper, int *pivot)
{
	double x = v[lower];
	int up = lower + 1; /* index will go up */
	int down = upper;   /* index will go down */
	while (up < down) {
		while ((up < down) && (v[up] <= x))
			up++;
		while ((up < down) && (v[down] > x))
			down--;
		if (up == down)
			break;
		double tmp = v[up];
		v[up] = v[down];
		v[down] = tmp;
	}
	if (v[up] > x)
		up--;
	v[lower] = v[up];
	v[up] = x;
	*pivot = up;
}
static inline void quicksort(double *v, int start, int end)
{
	if (start < end) {
		int pivot;
		partition(v, start, end, &pivot);
		quicksort(v, start, pivot - 1);
		quicksort(v, pivot + 1, end);
	}
}
int main()
{
	omp_set_num_threads(8);
	size_t pivot = NUM_ELEMS;
	clock_t start, stop;
	double v[NUM_ELEMS];
        int n = NUM_ELEMS - 1;

	srand(time(NULL));
	for (int i = 0; i < NUM_ELEMS; i++)
		v[i] = rand() % NUM_ELEMS - 100;

#pragma omp parallel
	start = clock();
        quicksort(&v[0], 0, NUM_ELEMS - 1);
	stop = clock();

	printf("%.4lf\n", (stop - start) / ((double)CLOCKS_PER_SEC));
}
