#include "ivm.hpp"
#include <algorithm>
#include <vector>

IVM::IVM(int n) {
    I = 0;
    V.resize(n, 0);
    M.resize(n);
    direction.resize(n, 0); // Assuming forward branching as specified
    
    // Initialize the first row with all jobs (1 to n)
    M[0].resize(n);
    for (int i = 0; i < n; i++) {
        M[0][i] = i + 1; // Jobs are 1-indexed
    }
}

void IVM::backtrack() {
    if (I > 0) {
        I--; // Move up one level
        V[I]++; // Move to the next sibling node
        
        // If we've reached the end of the current level's row, keep backtracking
        while (I >= 0 && V[I] >= static_cast<int>(M[I].size())) {
            I--;
            if (I >= 0) {
                V[I]++;
            }
        }
    } else if (I == 0 && V[0] < static_cast<int>(M[0].size()) - 1) {
        // At root level, just move to the next job
        V[0]++;
    } else {
        // We've exhausted all possibilities
        I = -1;
    }
}

void IVM::branch(const std::vector<int>& J, const std::vector<int>& lb1_values) {
    // Create pairs of jobs and their LB1 values
    std::vector<std::pair<int, int>> job_lb_pairs;
    for (size_t i = 0; i < J.size(); i++) {
        job_lb_pairs.push_back({J[i], lb1_values[i]});
    }
    
    // Sort by lower bound (DFLLB - Depth-First Least-Lower-Bound)
    std::sort(job_lb_pairs.begin(), job_lb_pairs.end(), 
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  return a.second < b.second; // Sort by LB1 value
              });
    
    I++; // Move to the next level
    V[I] = 0; // Start at the first child
    
    // Fill the next row of matrix M with unscheduled jobs
    M[I].resize(job_lb_pairs.size());
    for (size_t i = 0; i < job_lb_pairs.size(); i++) {
        M[I][i] = job_lb_pairs[i].first;
    }
}

std::vector<int> IVM::getCurrentSigma1() const {
    std::vector<int> sigma1;
    
    // Collect all scheduled jobs up to current level I
    for (int level = 0; level <= I; level++) {
        if (level < static_cast<int>(V.size()) && V[level] < static_cast<int>(M[level].size())) {
            int job = M[level][V[level]];
            // Take absolute value in case job was marked as pruned
            sigma1.push_back(std::abs(job));
        }
    }
    
    return sigma1;
}

std::vector<int> IVM::getCurrentJ() const {
    std::vector<int> J;
    
    // Get all unscheduled jobs at current level I
    if (I >= 0 && I < static_cast<int>(M.size())) {
        for (size_t j = V[I] + 1; j < M[I].size(); j++) {
            int job = M[I][j];
            if (job > 0) { // Only include non-pruned jobs
                J.push_back(job);
            }
        }
    }
    
    return J;
}

bool IVM::isComplete() const {
    // Check if we're at the deepest level and have no more unscheduled jobs
    return I >= 0 && I == static_cast<int>(M.size()) - 1 && 
           V[I] == static_cast<int>(M[I].size()) - 1;
}

bool IVM::hasNext() const {
    // Check if there are more nodes to explore
    return I >= 0 && (
        // More siblings at current level
        (V[I] < static_cast<int>(M[I].size()) - 1) ||
        // Or more levels to backtrack to
        (I > 0)
    );
}
