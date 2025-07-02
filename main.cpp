#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "Calculateur.hpp"
#include "Intrinsics.hpp"
#include "IntrinsicsAVX.hpp"
#include "IntrinsicsSSE42.hpp"


namespace po = boost::program_options;

double scalar_prod_nul(double* u, double* v, long long unsigned N) {
    double result = 0.0;
    for(long long int i=0; i<N; i++) {
        result += u[i] * v[i];
    }
    return result;
}

double sum_nul(double* u, long long unsigned N) {
    double res = 0.0;
    for (long long unsigned i=0; i<N; i++) {
        res += u[i];
    }
    return res;
}



void test_sum(int N) {
    std::vector<double> u;

    for (int i=0; i<N; i++) {
        u.push_back(i);
    }

    clock_t t1 = clock();
    double s1 = sum_nul(u.data(), N);
    clock_t t2 = clock();
    double s2 = Calculateur<SSE42>::sum(u);
    clock_t t3 = clock();
    double s3 = Calculateur<AVX>::sum(u);
    clock_t t4 = clock();


    std::cout << "Somme réelle = " << ((double)N)*(N-1) / 2 << '\n';
    std::cout << "Somme 1 = " << s1 << " calculée en " << (t2 - t1) << " ticks\n";
    std::cout << "Somme 2 = " << s2 << " calculée en " << (t3 - t2) << " ticks\n";
    std::cout << "Somme 3 = " << s3 << " calculée en " << (t4 - t3) << " ticks\n";
}


void test_scalar_prod(int N) {

    std::vector<double> u,v;

    for (int i=0; i<N; i++) {
        u.push_back(i);
        v.push_back(i);
    }
    
    clock_t t1 = clock();
    double s1 = scalar_prod_nul(u.data(), v.data(), N);
    clock_t t2 = clock();
    double s2 = Calculateur<SSE42>::dot(u, v);
    clock_t t3 = clock();
    double s3 = Calculateur<AVX>::dot(u, v);
    clock_t t4 = clock();

    std::cout << "Somme réelle = " << ((double) N)*(N-1)*(2*N-1) / 6 << '\n';
    std::cout << "Somme 1 = " << s1 << " calculée en " << (t2 - t1) << " ticks\n";
    std::cout << "Somme 2 = " << s2 << " calculée en " << (t3 - t2) << " ticks\n";
    std::cout << "Somme 3 = " << s3 << " calculée en " << (t4 - t3) << " ticks\n";
}


int main(int argc, char** argv) {

    po::options_description desc;
    desc.add_options()
        ("help", "produce help message")
        ("set-intrin", po::value<std::string>(),  
            "Set a specification of the intrinsics instruction set you want to use. If not specified, will default to the maximum available on the machine.");


    po::variables_map args;
    try {
        po::store(po::parse_command_line(argc, argv, desc), args);
    } catch (po::unknown_option e) {
        std::cout << e.what() << "\n";
    }

    po::notify(args);

    if (args.count("help")) {
        std::cout << desc << '\n';
        return 1;
    }

    try {

        int instructionsSet = setIntrinsics((args.count("set-intrin") ? args["set-intrin"].as<std::string>() : "default"));

        std::cout << "Instructions = " << instructionsSet << '\n';

    } catch (std::invalid_argument e) {
        std::cerr << e.what() << '\n';
    } catch (std::exception e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}