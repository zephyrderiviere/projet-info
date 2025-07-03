#ifndef INTRINSICS
#define INTRINSICS

#include <boost/algorithm/string/predicate.hpp>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <boost/algorithm/string.hpp>

enum InstructionsSet {
	NONE = -1,
	AVX512,
	AVX2,
	AVX,
	SSE42,
	SSE2,
};


inline std::ostream& operator<<(std::ostream& o, InstructionsSet set) {
	switch(set) {
		case NONE : 	return o << "NONE";
		case AVX512:	return o << "AVX512";
		case AVX2:		return o << "AVX2";
		case AVX:		return o << "AVX";
		case SSE42:		return o << "SSE4.2";	 
		case SSE2:		return o << "SSE2";	 
	}

	return o;
}


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


inline InstructionsSet setIntrinsics(std::string const& flag) {
	if (flag == "default") {
		if (__builtin_cpu_supports("avx")) {
      	   	return AVX;
		    }
		if (__builtin_cpu_supports("sse4.2")) {
			return SSE42;
    	}
		std::cerr << "No intrinsics detected. Using regular double operations\n";
        return NONE;
    } 
	if (boost::iequals(flag, "SSE4.2")) {
        //We check if the CPU supports the requested flag
		return (__builtin_cpu_supports("sse4.2"))  ? SSE42 : throw std::invalid_argument("Your CPU doesn't support SSE4.2 instructions.");
	} 
	if (boost::iequals(flag, "AVX")) {
        //We check if the CPU supports the requested flag
		return (__builtin_cpu_supports("avx"))     ? AVX   : throw std::invalid_argument("Your CPU doesn't support AVX instructions.");
    }
	if (boost::iequals(flag, "none")) {
        return NONE;  
    }

	throw std::invalid_argument("Not a valid intrinsics instructions set. Program Aborted.");
}

#endif
