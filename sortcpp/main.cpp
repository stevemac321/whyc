#include <omp.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>

const int NUM_ELEMS = 1000000;

int main()
{
	std::vector<int> v(NUM_ELEMS);

	srand(time(NULL));
	for (int i = 0; i < NUM_ELEMS; i++)
		v[i] = rand() % NUM_ELEMS - 1;

#pragma omp parallel
	clock_t start = clock();
	std::sort(std::begin(v), std::end(v));

	clock_t stop = clock();
	std::cout << ((stop - start) / ((double)CLOCKS_PER_SEC)) << "\n";
}
