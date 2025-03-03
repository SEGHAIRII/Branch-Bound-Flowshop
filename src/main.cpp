#include "pfsp_instance.hpp"
#include "bnb.hpp"
#include <iostream>
#include <string>

int main() {
    std::string filepath;
    std::cout << "Enter the path to the PFSP instance file: ";
    std::getline(std::cin, filepath);

    PFSPInstance instance = readInstance(filepath);
    // BnBResult result = branchAndBound(instance);
    BnBResult result;
    result.schedule = {1, 2, 0}; // Example schedule (job order)
    result.upper_bound = 25;
    result.lower_bound = 20;
    result.makespan = 25;
    std::cout << "Jobs: " << instance.n << ", Machines: " << instance.m << std::endl;
    for (const auto& row : instance.p) {
        for (int value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    printSolution(instance, result);
    return 0;
}