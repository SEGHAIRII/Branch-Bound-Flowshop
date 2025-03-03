#include "bnb.hpp"

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

#include "ivm.hpp"
#include "lower_bound.hpp"
#include "pfsp_instance.hpp"
#include "utils.hpp"

BnBResult branchAndBound(const PfspInstance& instance) {

    int n = instance.getNbJob();
    int m = instance.getNbMac();
    
    BnBResult bestSolution;

    bestSolution.lower_bound = std::numeric_limits<int>::max(); 
    bestSolution.makespan = INT_MAX;
    bestSolution.schedule.resize(n);
    
    IVM ivm(n);
    
    int initialSolution = nehHeuristic(instance);
    int UB = initialSolution;
    bestSolution.upper_bound = UB;  
    bestSolution.makespan = initialSolution;
    
    std::cout << "Initial UB from NEH: " << UB << std::endl;
    
    // initialize counts for statistics
    long long nodesExplored = 0;
    long long nodesPruned = 0;
    
    // create vectors to store head and tail completion times
    std::vector<int> headCompletionTimes(m + 1, 0);
    std::vector<int> tailCompletionTimes(m + 1, 0);
    
    // continue exploration until the IVM structure is empty (all nodes explored or pruned)
    while (ivm.I >= 0) { 
        nodesExplored++;
        
        // Get the current level from IVM
        int level = ivm.I;
        
        // Check if we have reached a leaf node (complete solution)
        if (level == n) {
            // Extract the complete permutation from IVM
            std::vector<int> currentSequence = ivm.getCurrentSigma1();
            
            // Compute the makespan for this solution
            int makespan = computeCompletion(currentSequence, instance.m - 1, instance);
            
            // Update the best solution if needed
            if (makespan < UB) {
                UB = makespan;
                bestSolution.upper_bound = UB;
                bestSolution.makespan = makespan;
                bestSolution.schedule = currentSequence;
            
                // Récupérer le vecteur des lower bounds
                std::vector<int> lb1_values = computeLB1(currentSequence, {}, {}, instance);
                
                // Prendre la plus petite valeur comme lower bound optimal
                bestSolution.lower_bound = *std::min_element(lb1_values.begin(), lb1_values.end());
            
                std::cout << "New UB: " << UB << " | Updated LB1: " << bestSolution.lower_bound << std::endl;
            }
            
            
            // Backtrack to explore other parts of the tree
            ivm.backtrack();
            continue;
        }
        
        // Extract current partial schedules (σ1, σ2) and unscheduled jobs J
        std::vector<int> sigma1, sigma2, unscheduledJobs;

        sigma1 = ivm.getCurrentSigma1();
        unscheduledJobs = ivm.getCurrentJ();
        sigma2 = {}; // fel forward branching makach sigma2

        
        // Compute head completion times for the current node (forward branching)
        headCompletionTimes.clear();
        for (int k = 0; k < instance.m; ++k) {
            headCompletionTimes.push_back(computeCompletion(sigma1, k, instance));
        }
        
        // calculer les temps de complétion tail, kamel à zéro en forward branching
        tailCompletionTimes.assign(instance.m, 0);

        
        // Compute LB1 for all children using forward branching
        std::vector<int> childrenLB1(unscheduledJobs.size());
        childrenLB1 = computeLB1(sigma1, sigma2, unscheduledJobs, instance);

        
        // Create a vector of indices for jobs to sort by lower bound
        std::vector<int> indices(unscheduledJobs.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            indices[i] = i;
        }
        
        // Sort indices by lower bound in non-decreasing order
        sort_by_lower_bound(indices, childrenLB1, unscheduledJobs);
        
        // Process each unscheduled job in order of increasing lower bound
        bool allPruned = true;
        for (size_t i = 0; i < indices.size(); ++i) {
            int jobIndex = indices[i];
            int job = unscheduledJobs[jobIndex];
            int lb1 = childrenLB1[jobIndex];
            
            // pruning
            if (lb1 >= UB) {
                nodesPruned++;
                continue;
            }
            
            // This child node is promising, add it to IVM
            allPruned = false;
            // ivm.addChild(job, true);
            // Ajouter le job à la séquence partielle (sigma1)
            ivm.V.push_back(job); 
            
            // Avancer au niveau suivant (branchement en avant)
            ivm.I++; 
            
            // Mettre à jour M pour refléter les jobs non encore programmés
            ivm.M[ivm.I] = ivm.M[ivm.I - 1]; 
            ivm.M[ivm.I].erase(std::remove(ivm.M[ivm.I].begin(), ivm.M[ivm.I].end(), job), ivm.M[ivm.I].end());
        }
        
        // If all children were pruned, backtrack
        if (allPruned) {
            ivm.backtrack();
        }
    }
    
    // Print statistics
    std::cout << "Nodes explored: " << nodesExplored << std::endl;
    std::cout << "Nodes pruned: " << nodesPruned << std::endl;
    
    return bestSolution;
}

// Helper function to sort indices by lower bound in non-decreasing order
// If two nodes have the same lower bound, break the tie by the job with less idle time
void sort_by_lower_bound(
    std::vector<int>& indices, 
    const std::vector<int>& lowerBounds,
    const std::vector<int>& jobs
) {
    // Sort indices based on the lower bounds
    std::sort(indices.begin(), indices.end(), [&lowerBounds, &jobs](int a, int b) {
        // Primary sort by lower bound
        if (lowerBounds[a] != lowerBounds[b]) {
            return lowerBounds[a] < lowerBounds[b];
        }
        // In a real implementation this would use the idle time computation as mentioned in the paper for simplicity, I used the job index as a proxy
        return jobs[a] < jobs[b];
    });
}