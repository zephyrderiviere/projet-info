#ifndef INTRINSICS_SSE42
#define INTRINSICS_SSE42

#include "Intrinsics.hpp"
#include <emmintrin.h>
#include <nmmintrin.h>

template<> struct instructionsSetTraits<SSE42> {
	using type = __m128d;
	static const size_t size = 2;
};

template<> class IntrensicsInterface<SSE42> {
	public:
		using vect_t = typename instructionsSetTraits<SSE42>::type;
		
		static const size_t vect_sz = instructionsSetTraits<SSE42>::size;
		
		inline static vect_t zeros() {
			return _mm_set_pd(0,0);
		}
		
		inline static vect_t load(double* d) {
			return _mm_load_pd(d);
		}
		inline static vect_t loadu(double* d) {
			return _mm_loadu_pd(d);
		}
		
		inline static vect_t add(vect_t a, vect_t b) {
			return _mm_add_pd(a, b);
		}

		inline static vect_t mul(vect_t a, vect_t b) {
			return _mm_mul_pd(a, b);
		}
		
		inline static void store(double* d, vect_t a) {
			_mm_store_pd(d, a);
		}

		inline static void storeu(double* d, vect_t a) {
			_mm_storeu_pd(d, a);
		}
};

#endif
