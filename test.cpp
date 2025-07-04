#define BOOST_TEST_MODULE TestAddition
#include <boost/test/included/unit_test.hpp>
#include "Calculateur.hpp"

template <int instructionsSet>
void testAdd() {
	int SIZE = 10000;

	double* a = (double*)aligned_alloc(64, SIZE * sizeof(double));
	double* b = (double*)aligned_alloc(64, SIZE * sizeof(double));
	double* res = (double*)aligned_alloc(64, SIZE * sizeof(double));
	
	for(int i = 0; i < SIZE; i++) {
		a[i] = (double) i;
		b[i] = (double) 2*i;
	}

	Calculateur<instructionsSet>::add(a,b,res,SIZE);
	for(int i=0; i<SIZE; i++) {
		BOOST_CHECK_EQUAL(res[i], 3*i);
	}
	
	free(a);
	free(b);
	free(res);
}

BOOST_AUTO_TEST_CASE(TestAdditionAVX) {
	testAdd<AVX>();
}

BOOST_AUTO_TEST_CASE(TestAdditionSSE42) {

	testAdd<SSE42>();
}
