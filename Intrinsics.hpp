#ifndef INTRINSICS
#define INTRINSICS

#include <boost/algorithm/string/predicate.hpp>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <boost/algorithm/string.hpp>

enum InstructionsSet {
	AVX512,
	AVX2,
	AVX,
	SSE42,
	SSE2
};


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
		static vect_t loadu(double* d);
		
		static vect_t add(vect_t a, vect_t b);

		static vect_t mul(vect_t a, vect_t b);
		
		static void store(double* d, vect_t a);
		static void storeu(double* d, vect_t a);

};


inline int setIntrinsics(std::string const& flag) {
	if (boost::iequals(flag, "default")) {
		if (__builtin_cpu_supports("avx2")) {
      		return AVX2;
		} else if (__builtin_cpu_supports("avx")) {
      		return AVX;
		} else if (__builtin_cpu_supports("sse4.2")) {
			return SSE42;
		} else if (__builtin_cpu_supports("sse2")) {
      		return SSE2;
		} else {
			throw std::invalid_argument("No Intrinsics detected. Program Aborted.");
		}
	}
	if (boost::iequals(flag, "SSE2")) {
		if (__builtin_cpu_supports("sse2")) {
			return SSE2;
		} else {
			throw std::invalid_argument("Your CPU doesn't support SSE2 instructions.");
		}
	}
	if (boost::iequals(flag, "SSE4.2")) {
		if (__builtin_cpu_supports("sse4.2")) {
			return SSE42;
		} else {
			throw std::invalid_argument("Your CPU doesn't support SSE4.2 instructions.");
		}
	}
	if (boost::iequals(flag, "AVX")) {
		if (__builtin_cpu_supports("avx")) {
			return AVX;
		} else {
			throw std::invalid_argument("Your CPU doesn't support AVX instructions.");
		}
	}
	if (boost::iequals(flag, "AVX2")) {
		if (__builtin_cpu_supports("avx2")) {
			return AVX2;
		} else {
			throw std::invalid_argument("Your CPU doesn't support AVX2 instructions.");
		}
	}

	throw std::invalid_argument("Not a valid intrinsics instructions set. Program Aborted.");

	return -1;
}

#endif
