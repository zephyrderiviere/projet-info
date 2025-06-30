#include <boost/program_options/errors.hpp>
#include <stdio.h>
#include <boost/program_options.hpp>
#include <iostream>


namespace po = boost::program_options;


int main(int argc, char** argv) {

    po::options_description desc;
    po::variables_map args;
    try {
        po::store(po::parse_command_line(argc, argv, desc), args);
    } catch (po::unknown_option e) {
        std::cout << e.what() << "\n";
    }

    po::notify(args);
   

    printf("Smooth criminal\n");
    return 0;
}