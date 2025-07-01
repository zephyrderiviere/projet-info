#ifndef INTRINSICS_AVX
#define INTRINSICS_AVX

#include "Intrinsics.hpp"
#include <immintrin.h>

template<> struct instructionsSetTraits<AVX> {
	using type = __m256d;
	static const size_t size = 4;
};

template<> class IntrensicsInterface<AVX> {
	public:
		using vect_t = typename instructionsSetTraits<AVX>::type;
		
		static const size_t vect_sz = instructionsSetTraits<AVX>::size;
		
		static vect_t zeros() {
			return _mm256_set_pd(0,0,0,0);
		}
		
		static vect_t load(double* d) {
			return _mm256_load_pd(d);
		}
		
		static vect_t add(vect_t a, vect_t b) {
			return _mm256_add_pd(a, b);
		}
		
		static void store(double* d, vect_t a) {
			_mm256_store_pd(d, a);
		}
};

#endif
