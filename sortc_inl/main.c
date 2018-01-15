#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int NUM_ELEMS = 1000000;

static inline int compare(const void *x, const void *y)
{
	return *(int *)x - *(int *)y;
}
static inline void swap(int v[], int i, int j)
{
        int tmp;
        tmp = v[i];
        v[i] = v[j];
        v[j] = tmp;
}
static inline void quicksort(int v[], int left, int right)
{
        int i, last;
        if(left >= right)
                return;

        swap(v, left, (left + right) / 2);
        last = left;

        for(i = left + 1; i <= right; i++)
                if(v[i] < v[left])
                        swap(v, ++last, i);

        swap(v, left, last);
        quicksort(v, left, last-1);
        quicksort(v, last+1, right);
}
int main()
{
	size_t pivot = NUM_ELEMS;
	clock_t start, stop;
	int  v[NUM_ELEMS];
        int n = NUM_ELEMS - 1;

	srand(time(NULL));
	for (int i = 0; i < NUM_ELEMS; i++)
		v[i] = rand() % NUM_ELEMS - 100;

	start = clock();
        quicksort(v, 0, NUM_ELEMS - 1);
	stop = clock();

	printf("%.4lf\n", (stop - start) / ((double)CLOCKS_PER_SEC));
}
