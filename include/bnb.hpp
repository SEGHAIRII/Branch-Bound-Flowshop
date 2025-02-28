#ifndef BNB_HPP
#define BNB_HPP

#include "pfsp_instance.hpp"
#include "ivm.hpp"

struct BnBResult {
    std::vector<int> schedule; // Optimal job permutation
    int upper_bound;           // Best makespan found
    int lower_bound;           // LB1 of the optimal schedule
    int makespan;              // Same as upper_bound
};

BnBResult branchAndBound(PFSPInstance& instance);

#endif