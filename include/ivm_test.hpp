#ifndef IVM_TEST_HPP
#define IVM_TEST_HPP

#include "ivm.hpp"
#include <vector>
#include <string>

// Helper function to print a vector
void printVector(const std::vector<int>& vec, const std::string& label);

// Helper function to print the current state of an IVM instance
void printIVM(const IVM& ivm);

// Test case to mimic the example from Figure 1 in the documentation
void testIVM(int n);

#endif