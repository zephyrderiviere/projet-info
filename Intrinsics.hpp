#ifndef INTRINSICS
#define INTRINSICS

#include <cstddef>
#define AVX 0
#define SSE42 1

template <int instructionsSet> struct reg_undef_t {};

template <int instructionsSet> struct instructionsSetTraits {
	using type = reg_undef_t<instructionsSet>;
	static const size_t size = -1;
};

template <int instructionsSet>
class IntrensicsInterface {
	public:
		using vect_t = typename instructionsSetTraits<instructionsSet>::type;
		
		static const size_t vect_sz = instructionsSetTraits<instructionsSet>::size;
		
		static vect_t zeros();
		
		static vect_t load(double* d);
		
		static vect_t add(vect_t a, vect_t b);
		
		static void store(double* d, vect_t a);

};

#endif
