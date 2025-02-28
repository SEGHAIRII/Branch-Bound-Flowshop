#include "pfsp_instance.hpp"
#include "bnb.hpp"
#include <iostream>
#include <string>

int main() {
    std::string filepath;
    std::cout << "Enter the path to the PFSP instance file: ";
    std::getline(std::cin, filepath);

    PFSPInstance instance = readInstance(filepath);
    BnBResult result = branchAndBound(instance);

    printSolution(instance, result);

    return 0;
}