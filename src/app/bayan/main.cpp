#include <iostream>

#include "bayan.hpp"


int main(int argc, char* argv[])
{
    try {
        Bayan bayan;
        bayan.run(argc, argv);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}