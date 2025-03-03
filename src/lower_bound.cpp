#include "lower_bound.hpp"
#include <vector>
#include <algorithm>
#include <limits>

std::vector<int> computeLB1(const std::vector<int>& sigma1, 
                            const std::vector<int>& sigma2, 
                            const std::vector<int>& J, 
                            const PFSPInstance& instance) {
    int n = instance.n;
    int m = instance.m;
    const auto& p = instance.p;  // Processing times matrix

    std::vector<int> lb1_values; // Lower bound values for each job in J

    for (int j : J) {  // Iterate over each unscheduled job
        std::vector<int> r(m, 0);  // Release times
        std::vector<int> q(m, 0);  // Completion times
        std::vector<int> p_sum(m, 0); // Total processing time on each machine

        // Compute r(k) (release time for each machine)
        if (!sigma1.empty()) {
            for (int k = 0; k < m; k++) {
                int last_job = sigma1.back(); // Last job in sigma1
                r[k] = (k == 0) ? p[last_job][k] : r[k - 1] + p[last_job][k];
            }
        } else { // If sigma1 is empty, compute minimum start time based on remaining jobs
            for (int k = 0; k < m; k++) {
                r[k] = std::numeric_limits<int>::max();
                for (int job : J) {
                    int sum_p = 0;
                    for (int l = 0; l < k; l++)
                        sum_p += p[job][l];
                    r[k] = std::min(r[k], sum_p);
                }
            }
        }

        // Compute q(k) (completion time for each machine)
        if (!sigma2.empty()) {
            for (int k = m - 1; k >= 0; k--) {
                int first_job = sigma2.front(); // First job in sigma2
                q[k] = (k == m - 1) ? p[first_job][k] : q[k + 1] + p[first_job][k];
            }
        } else { // If sigma2 is empty, compute minimum completion time based on remaining jobs
            for (int k = m - 1; k >= 0; k--) {
                q[k] = std::numeric_limits<int>::max();
                for (int job : J) {
                    int sum_p = 0;
                    for (int l = k + 1; l < m; l++)
                        sum_p += p[job][l];
                    q[k] = std::min(q[k], sum_p);
                }
            }
        }

        // Compute total remaining work p(k)
        for (int k = 0; k < m; k++) {
            for (int job : J)
                p_sum[k] += p[job][k];
        }

        // Compute LB1 for this job
        int lb1 = 0;
        for (int k = 0; k < m; k++) {
            lb1 = std::max(lb1, r[k] + p_sum[k] + q[k]);
        }

        lb1_values.push_back(lb1);
    }

    return lb1_values;
}
