#pragma once
#include <vector>
#include <iostream>
#include "BlackScholes.hpp"

struct OptionBatch {
    double T;  // Expiry
    double K;  // Strike
    double sig; // Volatility
    double r;   // Risk-free rate
    double S;   // Stock price
    double C;   // Expected call price
    double P;   // Expected put price
};

namespace BatchPricer {

    // Define the test batches
    inline std::vector<OptionBatch> get_test_batches() {
        return {
            {0.25, 65, 0.30, 0.08, 60, 2.13337, 5.84628},    // Batch 1
            {1.0, 100, 0.2, 0.0, 100, 7.96557, 7.96557},     // Batch 2
            {1.0, 10, 0.50, 0.12, 5, 0.204058, 4.07326},     // Batch 3
            {30.0, 100.0, 0.30, 0.08, 100.0, 92.17570, 1.24750} // Batch 4
        };
    }

    // Compute and print prices for all batches
    inline void price_all_batches() {
        auto batches = get_test_batches();
        for (const auto& batch : batches) {
            double computed_C = BlackScholes::call_price(batch.S, batch.K, batch.T, batch.r, batch.sig, batch.r); // b = r for stocks
            double computed_P = BlackScholes::put_price(batch.S, batch.K, batch.T, batch.r, batch.sig, batch.r);
            
            std::cout << "Batch: T=" << batch.T << ", K=" << batch.K 
                      << ", sig=" << batch.sig << ", r=" << batch.r 
                      << ", S=" << batch.S << "\n";
            std::cout << "  Call: Computed=" << computed_C << ", Expected=" << batch.C << "\n";
            std::cout << "  Put: Computed=" << computed_P << ", Expected=" << batch.P << "\n";
        }
    }
}