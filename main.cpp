#include <boost/program_options.hpp>
#include <ctime>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "Calculateur.hpp"
#include "Intrinsics.hpp"


namespace po = boost::program_options;


int main(int argc, char** argv) {

	boost::timer::auto_cpu_timer t;

    srand(0);

    po::options_description desc;
    desc.add_options()
        ("help", "Produce help message")
        ("size,s", po::value<int>(), "Set the size of the matricees for the program")
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

    int size = (args.count("size")) ? args["size"].as<int>() : 1000;

    try {
        //Looking for the appropriate intrinsics instructions set
        std::string flag = (args.count("set-intrin") ? args["set-intrin"].as<std::string>() : "default");
        InstructionsSet set = setIntrinsics(flag);


        std::cout << "Selected instructions set : " << set << '\n';

        
        switch(set) {
            case AVX: {
                Calculateur<AVX>::program(size);
                break;
            }
            case SSE42: {
                Calculateur<SSE42>::program(size);
                break;
            }

            case NONE: {
                control_program(size);
                break;
            }

            default: break;
        }

    } catch (std::invalid_argument e) {
        std::cerr << e.what() << '\n';
    } catch (std::exception e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}