#ifndef PFSP_INSTANCE_HPP
#define PFSP_INSTANCE_HPP

#include "bnb.hpp"
#include <vector>
#include <string>


struct PFSPInstance
{
    int n;                           // Number of jobs
    int m;                           // Number of machines
    std::vector<std::vector<int>> p; // Processing times

    PFSPInstance() : n(0), m(0) {}
};

PFSPInstance readInstance(const std::string &filename);
void printSolution(const PFSPInstance &instance, const BnBResult &result);

#endif