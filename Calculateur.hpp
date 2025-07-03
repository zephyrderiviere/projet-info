#ifndef CALCULATEUR
#define CALCULATEUR

#include "Intrinsics.hpp"
#include "IntrinsicsAVX.hpp"
#include "IntrinsicsSSE42.hpp"
#include <cfloat>
#include <climits>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <boost/timer/timer.hpp>


template <int instructionsSet>
class Calculateur {
	public:
		static inline void add(double* a, double* b, double* res, int N) {
			for(int i=0; i<N-IntrensicsInterface<instructionsSet>::vect_sz; i+=IntrensicsInterface<instructionsSet>::vect_sz) {
				typename IntrensicsInterface<instructionsSet>::vect_t va = IntrensicsInterface<instructionsSet>::load(&a[i]);
				typename IntrensicsInterface<instructionsSet>::vect_t vb = IntrensicsInterface<instructionsSet>::load(&b[i]);
				typename IntrensicsInterface<instructionsSet>::vect_t vsum = IntrensicsInterface<instructionsSet>::add(va, vb);
				IntrensicsInterface<instructionsSet>::store(&res[i], vsum);
			}
		}

		/*
			Réalise la somme de tous les élements de `vector`
		*/
		static inline double sum(std::vector<double>& vector) {
			double* a = vector.data();
			int N = vector.size();
			using vect_t = typename IntrensicsInterface<instructionsSet>::vect_t;
			long unsigned multiplier = IntrensicsInterface<instructionsSet>::vect_sz;

			if (N < 2*multiplier) {
				//N petit, la vectorisation est inutile
				double result = 0.0;
				for(int k=0; k<N; k++) {
					result += a[k];
				}
				return result;
			}
			
			//On initialise un buffer pour stocker les sommes petit à petit
			vect_t vbuffer = IntrensicsInterface<instructionsSet>::loadu(a + N%multiplier);

			//On réalise les sommes vectorisées
    		for (int k=N%multiplier + multiplier; k<N; k+= multiplier) {
            	vect_t va = IntrensicsInterface<instructionsSet>::loadu(a + k);
            	vbuffer = IntrensicsInterface<instructionsSet>::add(va, vbuffer);
        	}
			
			alignas(32) double buffer[multiplier];
			IntrensicsInterface<instructionsSet>::store(buffer, vbuffer);

			//On n'oublie pas les premiers élements du tableau (qui peut faire une taille non divisible par multiplier)
			double result = 0.0;
			for(int k=0; k<N % multiplier; k++) {
				result += a[k];
			}

			//On somme les éléments du buffer
    		for(int i=0; i<multiplier; i++) {
        		result += buffer[i];
    		}
    		return result;
		}

		/*
			Calcule le produit scalaire entre `v1` et `v2`. 
			Si les deux vecteurs sont de tailles différentes, on ignore les derniers élements du plus grand vecteur
		*/
		static inline double dot(std::vector<double>& v1, std::vector<double>& v2) {
			int N = std::min(v1.size(), v2.size());

			using vect_t = typename IntrensicsInterface<instructionsSet>::vect_t;
			long unsigned multiplier = IntrensicsInterface<instructionsSet>::vect_sz;


			
			vect_t vbuffer = IntrensicsInterface<instructionsSet>::mul( IntrensicsInterface<instructionsSet>::loadu(v1.data() + N%multiplier), 
																		IntrensicsInterface<instructionsSet>::loadu(v2.data() + N%multiplier));

			//On réalise les sommes vectorisées
    		for (int k=N%multiplier + multiplier; k<N; k+= multiplier) {
            	vect_t va = IntrensicsInterface<instructionsSet>::loadu(v1.data() + k);
            	vect_t vb = IntrensicsInterface<instructionsSet>::loadu(v2.data() + k);
				vect_t prod = IntrensicsInterface<instructionsSet>::mul(va, vb);
            	vbuffer = IntrensicsInterface<instructionsSet>::add(prod, vbuffer);
        	}


			alignas(32) double buffer[multiplier];
			IntrensicsInterface<instructionsSet>::store(buffer, vbuffer);

			double result = 0.0;
			
			//On n'oublie pas les premiers élements du tableau (qui peut faire une taille non divisible par multiplier)
			for(int k=0; k<N % multiplier; k++) {
				result += v1[k] * v2[k];
			}

			//On somme les éléments du buffer
    		for(int i=0; i<multiplier; i++) {
        		result += buffer[i];
    		}
    		return result;
		}
		/*
			Calcule le produit matriciel entre `A` et la transposée de `B`. 
			L'algorithme vérifie les tailles des vecteurs, et renvoie une erreur `std::invalid_argument` si les tailles ne correspondent pas.
		*/
		static inline std::vector<std::vector<double>> matrixProduct(std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& B) {
			std::vector<std::vector<double>> result;
			int p = A.size();
			int q = B.size();
			int n = -1;

			for (int i=0; i<p; i++) {
				std::vector<double> row;
				for (int j=0; j<q; j++) {
					if (A[i].size() != B[j].size()) throw std::invalid_argument("Wrong sizes in the matrix product !");
					row.push_back(dot(A[i], B[j]));
				}
				result.push_back(row);
			}
			return result;
		}



		static void program(int n) {
			std::vector<std::vector<double>> A, B;

			for (int i=0; i<n; i++) {
				std::vector<double> rowA, rowB;
				for (int j=0; j<n; j++) {
					rowA.push_back(((double)rand() / INT_MAX) *  10);
					rowB.push_back(((double)rand() / INT_MAX) *  10);
				}
				A.push_back(rowA);
				B.push_back(rowB);
			}

			std::vector<std::vector<double>> result = matrixProduct(A, B);

			double res = 0.0;
			for (int i=0; i<n; i++) {
				res += sum(result[i]);
			}

			std::cout << res << '\n';
		}

};





void inline control_program(int n) {
	std::vector<std::vector<double>> A, B;

	for (int i=0; i<n; i++) {
		std::vector<double> rowA, rowB;
		for (int j=0; j<n; j++) {
			rowA.push_back(((double)rand() / INT_MAX) *  10);
			rowB.push_back(((double)rand() / INT_MAX) *  10);
		}
		A.push_back(rowA);
		B.push_back(rowB);
	}
	std::vector<std::vector<double>> result; 
	for (int i=0; i<n; i++) {
		std::vector<double> row;
		for (int j=0; j<n; j++) {
			double prod = 0.0;
			for (int k=0; k<n; k++) {
				prod += A[i][k] * B[j][k];
			}
			row.push_back(prod);
		}
		result.push_back(row);
	}
	double res = 0.0;
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			res += result[i][j];
		}
	}
	std::cout << res << '\n';
}

#endif //CALCULATEUR