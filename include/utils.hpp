#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include "pfsp_instance.hpp"

// Computes the completion time of a job sequence up to machine k (0-based index)
// Returns the time when the last job in seq finishes on machine M_{k+1}
// Used for LB1 (r(sigma1, k)) and evaluating complete solutions' makespans when k = m-1
//tlgaha f section 3.4
int computeCompletion(const std::vector<int>& seq, int k, const PFSPInstance& instance);

// Implements the Nawaz-Enscore-Ham (NEH) heuristic to find an initial solution
// Returns the makespan of a good permutation, used as the initial upper bound (UB)
// Builds the sequence by sorting jobs by total processing time and inserting optimally
int nehHeuristic(const PFSPInstance& instance);

#endif