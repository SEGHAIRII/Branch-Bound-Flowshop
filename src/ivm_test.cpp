#include "ivm_test.hpp"
#include <iostream>
#include <iomanip>  // For formatting output
#include <cassert>
#include <cstdlib>
#include <ctime>

// Helper function to print a vector
void printVector(const std::vector<int>& vec, const std::string& label) {
    std::cout << label << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// Helper function to print the current state of IVM
void printIVMState(const IVM& ivm) {
    std::cout << "\n======== IVM STATE ========\n";
    std::cout << "Current Level (I): " << ivm.I << std::endl;
    
    std::cout << "V: [";
    for (size_t i = 0; i <= ivm.I; ++i) {
        std::cout << ivm.V[i];
        if (i < ivm.I) std::cout << ", ";
    }
    std::cout << "]\n";

    std::cout << "M:\n";
    for (size_t i = 0; i <= ivm.I; ++i) {
        std::cout << "  Level " << i << ": [";
        for (size_t j = 0; j < ivm.M[i].size(); ++j) {
            std::cout << ivm.M[i][j];
            if (j < ivm.M[i].size() - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }

    printVector(ivm.getCurrentSigma1(), "Current Sigma1 (Scheduled Jobs)");
    printVector(ivm.getCurrentJ(), "Remaining Jobs (J)");

    std::cout << "isComplete: " << (ivm.isComplete() ? "true" : "false") << std::endl;
    std::cout << "hasNext: " << (ivm.hasNext() ? "true" : "false") << std::endl;
    std::cout << "============================\n\n";
}

// Test case
void testIVM(int n) {
    std::cout << "Initializing IVM with " << n << " jobs...\n";
    IVM ivm(n);
    std::srand(std::time(0)); // Seed for random lb1 values

    while (ivm.hasNext()) {
        // Print the state of IVM
        printIVMState(ivm);

        std::vector<int> sigma1 = ivm.getCurrentSigma1();
        std::vector<int> J = ivm.getCurrentJ();

        if (J.empty()) {
            std::cout << "🔄 Branch is complete, backtracking...\n";
            ivm.backtrack();
        } else {
            // Generate random lower bound values for jobs
            std::vector<int> lb1_values(J.size());
            for (size_t i = 0; i < J.size(); i++) {
                lb1_values[i] = std::rand() % 100; // Random values between 0-99
            }

            // Print generated LB1 values
            std::cout << "🔀 Branching with jobs and generated LB1 values:\n";
            for (size_t i = 0; i < J.size(); ++i) {
                std::cout << "  Job " << J[i] << " -> LB1: " << lb1_values[i] << "\n";
            }
            std::cout << "\n";

            ivm.branch(J, lb1_values);
        }

        if (ivm.isComplete()) {
            std::cout << "✅ Solution found!\n";
            break;
        }
    }

    std::cout << "🎯 Search finished.\n";
}

int main() {
    testIVM(5);
    std::cout << "All tests completed successfully!" << std::endl;
    return 0;
}
