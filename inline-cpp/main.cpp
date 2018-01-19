#include <time.h>
#include <list>
#include <iostream>

const int NUM_ELEMS = 1000000;

int main()
{
        std::list<int> l;
        int a[NUM_ELEMS];

	srand(time(NULL));
	for (int i = 0; i < NUM_ELEMS; i++)
		a[i] = rand() % NUM_ELEMS - 1;

	clock_t start = clock();
        for(int i=0; i < NUM_ELEMS; i++)
                l.push_front(a[i]);

	clock_t stop = clock();
	std::cout << ((stop - start) / ((double)CLOCKS_PER_SEC)) << "\n";
}
