#ifndef IVM_HPP
#define IVM_HPP

#include <vector>

struct IVM {
    int I;                  // Current level
    std::vector<int> V;     // Position vector
    std::vector<std::vector<int>> M; // Job matrix
    std::vector<int> direction;     // Direction vector, rana khadmin only forward branching so it should be all 0s

    // Constructor: Initializes the IVM for n jobs, setting up the root level (I=0) with all jobs available
    IVM(int n);

    // New constructor: Initializes the IVM for n jobs, sorting them by lb1_values
    IVM(int n, const std::vector<int>& lb1_values);

    // Moves the search back to a previous level when the current level's subproblems are exhausted
    // Updates I and V to point to the next unexplored node or terminate if none remain
    void backtrack();

    // Advances to the next level by branching from the current subproblem
    // Takes unscheduled jobs (J) and their LB1 values, sorts them by LB1 for DFLLB, and updates M[I+1]
    void branch(const std::vector<int>& J, const std::vector<int>& lb1_values);

    // Returns the current partial schedule (sigma1) up to level I
    // Builds the sequence of jobs chosen so far based on V and M
    std::vector<int> getCurrentSigma1() const;

    // Returns the unscheduled jobs (J) at the current level I
    // Extracts jobs from M[I] that haven’t been scheduled yet (after V[I])
    std::vector<int> getCurrentJ() const;

    // Checks if the current subproblem is a complete permutation (all jobs scheduled)
    // Returns true if no unscheduled jobs remain at level I
    bool isComplete() const;

    // Determines if there are more subproblems to explore at the current level
    // Returns true if I >= 0 and V[I] can increment to the next job in M[I]
    bool hasNext() const;
};

#endif