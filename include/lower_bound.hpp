#ifndef LOWER_BOUND_HPP
#define LOWER_BOUND_HPP

#include <vector>
#include "pfsp_instance.hpp"

std::vector<int> computeLB1(const std::vector<int>& sigma1, 
                            const std::vector<int>& sigma2, 
                            const std::vector<int>& J, 
                            const PFSPInstance& instance);

#endif