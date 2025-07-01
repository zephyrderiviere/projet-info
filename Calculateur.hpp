#ifndef CALCULATEUR
#define CALCULATEUR

#include "Intrinsics.hpp"
#include "IntrinsicsSSE42.hpp"
#include "IntrinsicsAVX.hpp"
#include <iostream>

template <int instructionsSet>
class Calculateur {
	public:
		static void add(double* a, double* b, double* res, int N) {
			for(int i=0; i<N-IntrensicsInterface<instructionsSet>::vect_sz; i+=IntrensicsInterface<instructionsSet>::vect_sz) {
				typename IntrensicsInterface<instructionsSet>::vect_t va = IntrensicsInterface<instructionsSet>::load(&a[i]);
				typename IntrensicsInterface<instructionsSet>::vect_t vb = IntrensicsInterface<instructionsSet>::load(&b[i]);
				typename IntrensicsInterface<instructionsSet>::vect_t vsum = IntrensicsInterface<instructionsSet>::add(va, vb);
				IntrensicsInterface<instructionsSet>::store(&res[i], vsum);
			}
		}

		static double sum(double* a, int N) {
			using vect_t = typename IntrensicsInterface<instructionsSet>::vect_t;
			long unsigned multiplier = IntrensicsInterface<instructionsSet>::vect_sz;

			

			double buffer[multiplier];
			for(int k=N%multiplier; k<multiplier + (N % multiplier); k++) {
				buffer[k] = a[k];
			}

    		for (int k=N%multiplier + multiplier; k<N; k+= multiplier) {
            	vect_t va = IntrensicsInterface<instructionsSet>::load(buffer);
            	vect_t vb = IntrensicsInterface<instructionsSet>::load(a + k);
            	vect_t sum = IntrensicsInterface<instructionsSet>::add(va, vb);
            	IntrensicsInterface<instructionsSet>::store(buffer, sum);
        	}

			double result = 0.0;
			for(int k=0; k<N % multiplier; k++) {
				result += a[k];
			}

    		for(int i=0; i<multiplier; i++) {
        		result += buffer[i];
    		}
    		return result;
		}

};

#endif
