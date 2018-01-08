all:
	gcc  main.c  -O3 -std=c11 -fopenmp -o sortc

asm:
	gcc -S -c  main.c  -O3 -std=c11 
dbg: 
	gcc main.c  -ggdb -O0 -std=c11 -fsanitize=address -Wall -o sortcdbg

clean:
	rm sortc*
