all:
	clang++ main.cpp -O3 -std=c++17 -o sortcpp
asm:
	clang++ -c -S main.cpp -O3 -std=c++17 

qsort:
	g++ main.cpp -O3 -std=c++17 -D_GLIBCXX_QUICKSORT -D_GLIBCXX_PARALLEL -fopenmp -o sortcpp
merge:
	g++ main.cpp -O3 -std=c++17 -D_GLIBCXX_QUICKSORT -D_GLIBCXX_PARALLEL -fopenmp -o sortcpp
clean:
	rm sortcpp
