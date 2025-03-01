#include "utils.hpp"

int computeCompletion(const std::vector<int> &seq, int k, const PFSPInstance &instance)
{
    // inorder to compute the completion time of sigma1 sequence up to the kth machine
    // we need to already have the completion time of sigma1 - last task up to kth   machine
    // as well as              the completion time of sigma1             up to k-1th machine
    // to avoid recursive calls that will generate redundant calculations
    // we will procede by creating a (seq.size() X k+1) matrix
    // the (i,j) element of that matrix will basically contain computeCompletion(seq[:i],j,instance)
    // you may notice that we have added an additional row and column
    // they will represent respectively the completion time of an empty sequence up to the kth machine
    // and the completion time of an any sequence up to "-1" machine -so no machines at all-
    // the special row and column will be always at 0
    std::vector<std::vector<int>> completionMatrix(seq.size() + 1, std::vector<int>((k + 1) + 1, 0));

    // now that the matrix is initialized, we can iterate row by row "ignoring the 0th row and 0th column"
    // total number of iterations : seq.size() x (k+1)
    for (int i = 1; i < seq.size() + 1; i++)
    {
        for (int j = 1; j < (k + 1) + 1; j++)
        {
            completionMatrix[i][j] = std::max(completionMatrix[i - 1][j], completionMatrix[i][j - 1]) + instance.p[seq[i - 1]][j - 1];
        }
    }

    return completionMatrix[seq.size()][k + 1];
}

int nehHeuristic(const PFSPInstance &instance)
{
    // indexedT[i].first == Total processing time for the task i
    // indexedT[i].second == The ith task index "which is i"
    std::vector<std::pair<int, int>> indexedT;

    int total = 0;

    // filling out indexedT
    for (int i = 0; i < instance.n; i++)
    {
        for (int j = 0; j < instance.m; j++)
        {
            total += instance.p[i][j];
        }
        indexedT.push_back({total, i});
        total = 0;
    }

    // sorting tasks by their total processing time "the default is acsending "
    std::sort(indexedT.begin(), indexedT.end());

    // best : the best sequence according to the Heuristic 'progressively built'
    std::vector<int> best = {};

    // fetch the two heaviest tasks from the sorted pair vector
    int heaviestTask = indexedT[indexedT.size() - 1].second;
    int secondHeaviestTask = indexedT[indexedT.size() - 2].second;

    // the value storing the makespan of the best permutation
    int bestValue = INT_MAX;

    std::vector<int> seq1 = {secondHeaviestTask, heaviestTask};
    std::vector<int> seq2 = {heaviestTask, secondHeaviestTask};

    // find the best combination of the first two tasks "static"
    int secondThenFirst = computeCompletion(seq1, instance.m - 1, instance);

    int firstThenSecond = computeCompletion(seq2, instance.m - 1, instance);
    if (secondThenFirst > firstThenSecond)
    {
        best = seq2;
        bestValue = firstThenSecond;
    }
    else
    {
        best = seq1;
        bestValue = secondThenFirst;
    }

    // iterating through the sorted tasks "from the heaviest in terms of total completion time to the lightest"
    for (int i = indexedT.size() - 3; i >= 0; i--)
    {
        // the current task to insert in the sequence
        int currentTask = indexedT[i].second;
        // make a separate copy of the current sequence inorder to try all of the different insertions on
        std::vector<int> currentBest = best;
        // reset the current best value
        int currentBestValue = INT_MAX;
        for (int j = 0; j <= best.size(); j++)
        {
            std::vector<int> tempVec = best;                  // Copy the original sequence
            tempVec.insert(tempVec.begin() + j, currentTask); // Insert at position j the currentTask "generate a possible configuration"

            int value = computeCompletion(tempVec, instance.m - 1, instance); // Evaluate the configuration
            if (value < currentBestValue)                                     // if configuration beats current best configuration
            {
                currentBestValue = value; // update the best currentValue
                currentBest = tempVec;    // update the best currentVector
            }
        }
        bestValue = currentBestValue; // update the bestValue
        best = currentBest;           // update the best vector
    }

    return bestValue;
}