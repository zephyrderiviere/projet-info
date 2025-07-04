#include <iostream>
#include <boost/chrono.hpp>
#include "Calculateur.hpp"

using namespace std;

int main(int argc, char* argv[]) {

	int SIZE = 10000000;
	if(argc>1) {
		SIZE = std::atoi(argv[1]);
	}

	double* a = (double*)aligned_alloc(64, SIZE * sizeof(double));
	double* b = (double*)aligned_alloc(64, SIZE * sizeof(double));
	double* res = (double*)aligned_alloc(64, SIZE * sizeof(double));

	for(int i = 0; i < SIZE; i++) {
		a[i] = (double) i;
		b[i] = (double) SIZE - i;
	}

	for(int s=100000; s<=SIZE; s*=10) {
		
		
		boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
		Calculateur<AVX>::add(a, b, res, s);
		boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
		std::cout << s <<' '<< sec.count() << ' ';
		
		start = boost::chrono::system_clock::now();
		Calculateur<SSE42>::add(a, b, res, s);
		sec = boost::chrono::system_clock::now() - start;
		std::cout << sec.count() << endl;
	}
	
	free(a);
	free(b);
	free(res);
	
	return 0;
}

