//Author: Andrew Kerr
//Date: 04/27/2022

//===[ INCLUDES ]==================================================================================================================
#include "graph.hpp"

//===[ MACROS ]====================================================================================================================

//===[ TYPEDEFS ]==================================================================================================================

//===[ GLOBALS ]===================================================================================================================

//===[ FUNCTIONS ]=================================================================================================================

int main(int argc, char* argv[]) {

    if(argc < 2) return -1;

    //Reads in every path from the file.
    Computers computers;
    {
        std::ifstream file(argv[1]);
        std::string name;
        int cost;
        while(file){
            file >> name;
            Computer* a = computers[name];
            if(!a) { a = new Computer(name.c_str()); computers += a; }
            file >> name;
            Computer* b = computers[name];
            if(!b) { b = new Computer(name.c_str()); computers += b; }
            file >> cost;
            if(!a || !b || !a->connect(*b, cost)) {
                std::cerr << "FATAL: Failed to allocate space for a computer or couldn't connect them together." << std::endl;
                exit(-3);
            }
        }
    }

    //std::cout << computers;

    //std::cout << "Attempting to solve for minimum path:" << std::endl;
    int minimum = solve(computers);
    std::cout << minimum << std::endl;

    return 0;
}