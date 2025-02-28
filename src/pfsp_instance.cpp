#include "pfsp_instance.hpp"
#include "bnb.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

PFSPInstance readInstance(const std::string& filename) {
    std::ifstream file(filename);
    PFSPInstance instance;

    if (!file) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return instance;
    }

    file >> instance.n >> instance.m;
    instance.p.resize(instance.m, std::vector<int>(instance.n));

    for (int i = 0; i < instance.m; ++i) {
        for (int j = 0; j < instance.n; ++j) {
            file >> instance.p[i][j];
        }
    }

    file.close();
    return instance;
}

void printSolution(const PFSPInstance& instance, const BnBResult& result) {
    // #todo Charlie
}