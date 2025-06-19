#pragma once
#include <cmath>
#include <functional>

class NumericalGreeks {
public:
    // Function type for pricing
    using PricingFunction = std::function<double(double, double, double, double, double, double)>;
    
    // Constructor with custom pricing function
    NumericalGreeks(PricingFunction pricing_fn, double h = 0.01)
        : pricing_fn_(pricing_fn), h_(h) {}
    
    // Set the step size
    void set_step_size(double h) { h_ = h; }
    
    // Delta approximation (central difference)
    double delta(double S, double K, double T, double r, double sigma, double b) {
        return (pricing_fn_(S + h_, K, T, r, sigma, b) - 
               pricing_fn_(S - h_, K, T, r, sigma, b)) / (2 * h_);
    }
    
    // Gamma approximation (central difference)
    double gamma(double S, double K, double T, double r, double sigma, double b) {
        return (pricing_fn_(S + h_, K, T, r, sigma, b) - 
               2 * pricing_fn_(S, K, T, r, sigma, b) + 
               pricing_fn_(S - h_, K, T, r, sigma, b)) / (h_ * h_);
    }
    
    // Theta approximation (backward difference)
    double theta(double S, double K, double T, double r, double sigma, double b) {
        return (pricing_fn_(S, K, T - h_, r, sigma, b) - 
               pricing_fn_(S, K, T, r, sigma, b)) / h_;
    }
    
private:
    PricingFunction pricing_fn_;
    double h_;
};