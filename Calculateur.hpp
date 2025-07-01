#ifndef CALCULATEUR
#define CALCULATEUR

#include "Intrinsics.hpp"
#include "IntrinsicsSSE42.hpp"
#include "IntrinsicsAVX.hpp"

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

};

#endif
