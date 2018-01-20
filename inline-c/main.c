/*==============================================================================
 Name        : main.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "list.inl"

const int NUM_ELEMS = 1000000;
struct nodeinl* ctor_int(void *data)
{
        assert(data);
        struct nodeinl *pnew = malloc(sizeof(struct nodeinl));
        assert(pnew);
        pnew->data = malloc(sizeof(int));
        assert(pnew->data);
        *(int*)pnew->data = *(int*)data; 
        return pnew;

}
void dtor_int(struct nodeinl *p)
{
        assert(p);
		free(p->data);
        free(p);
}
int main()
{
        struct listinl l;
        int a[NUM_ELEMS];
        clock_t start, stop;

	srand(time(NULL));
	for (int i = 0; i < NUM_ELEMS; i++)
		a[i] = rand() % NUM_ELEMS;

        listinl_init(&l, ctor_int, dtor_int);

        start = clock();
        for(int i=0; i < NUM_ELEMS; i++)
                listinl_addhead(&l, &a[i]);

	stop = clock();
        printf("%.4lf\n", (stop - start) / ((double) CLOCKS_PER_SEC));
       
        listinl_destroy(&l);
}



