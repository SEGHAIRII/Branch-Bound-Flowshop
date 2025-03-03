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

struct BnBResult {
    std::vector<int> schedule; // Optimal job permutation
    int upper_bound;           // Best makespan found
    int lower_bound;           // LB1 of the optimal schedule
    int makespan;              // Same as upper_bound
};

PFSPInstance readInstance(const std::string &filename);
void printSolution(const PFSPInstance &instance, const BnBResult &result);

#endif