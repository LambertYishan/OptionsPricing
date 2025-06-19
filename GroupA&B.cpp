#include <iostream>
#include <vector>
#include "BlackScholes.hpp"
#include "PutCallParity.hpp"
#include "MeshGenerator.hpp"
#include "BatchPricer.hpp"
#include "OptionPricingMatrix.hpp"
#include "Greeks.hpp"
#include "NumericalGreeks.hpp"
#include "AmericanOptions.hpp"
#include "AmericanOptionMatrix.hpp"

int main() {
    // Part a: Price options for Batch 1 to Batch 4
    std::cout << "=== Part a: Pricing Batches ===\n";
    BatchPricer::price_all_batches();

    // Part b: Put-call parity check
    std::cout << "\n=== Part b: Put-Call Parity ===\n";
    auto batches = BatchPricer::get_test_batches();
    for (const auto& batch : batches) {
        double parity_C = PutCallParity::call_from_put(batch.P, batch.S, batch.K, batch.T, batch.r);
        double parity_P = PutCallParity::put_from_call(batch.C, batch.S, batch.K, batch.T, batch.r);
        
        std::cout << "Batch: S=" << batch.S << ", K=" << batch.K << "\n";
        std::cout << "  Call (from parity): " << parity_C << " vs " << batch.C << "\n";
        std::cout << "  Put (from parity): " << parity_P << " vs " << batch.P << "\n";
        bool is_valid = PutCallParity::satisfies_parity(batch.C, batch.P, batch.S, batch.K, batch.T, batch.r);
        std::cout << "  Parity valid: " << (is_valid ? "Yes" : "No") << "\n";
    }

    // Part c: Mesh of S values (e.g., 10, 11, ..., 50)
    std::cout << "\n=== Part c: Mesh of S Values ===\n";
    auto S_mesh = MeshGenerator::linear_mesh(10.0, 50.0, 1.0);
    double K = 30.0, T = 1.0, r = 0.05, sig = 0.2, b = r;

    // Vector to store call prices
    std::vector<double> call_prices;
    call_prices.reserve(S_mesh.size());

    std::cout << "Call prices for S in [10, 50], K=30, T=1, r=0.05, sig=0.2:\n";
    for (double S : S_mesh) {
        double C = BlackScholes::call_price(S, K, T, r, sig, b);
        call_prices.push_back(C);
        std::cout << "S=" << S << ", C=" << C << "\n";
    }

    // Part d: Extended mesh generation for multiple parameters
    std::cout << "\n=== Part d: Mesh of every Value ===\n";
    
    // Varying S and K
    auto K_mesh = MeshGenerator::linear_mesh(20.0, 40.0, 2.0);
    auto price_matrix_SK = OptionPriceMatrix::compute_value_matrix(
        S_mesh, OptionPriceMatrix::ParamType::S,
        K_mesh, OptionPriceMatrix::ParamType::K,
        OptionPriceMatrix::GreekType::Price,
        0.0, 0.0, T, r, sig, b);
    
    print_value_matrix(price_matrix_SK, S_mesh, K_mesh, "S", "K", "Price");
    
    // Varying T and sigma
    auto T_mesh = MeshGenerator::linear_mesh(0.1, 2.0, 0.1);
    auto sig_mesh = MeshGenerator::linear_mesh(0.1, 0.5, 0.05);
    auto price_matrix_Tsig = OptionPriceMatrix::compute_value_matrix(
        T_mesh, OptionPriceMatrix::ParamType::T,
        sig_mesh, OptionPriceMatrix::ParamType::sig,
        OptionPriceMatrix::GreekType::Price,
        S_mesh[10], K, 0.0, r, 0.0, b);
    
    print_value_matrix(price_matrix_Tsig, T_mesh, sig_mesh, "T", "Sigma", "Price");
    
    // Varying r and sigma for puts
    auto r_mesh = MeshGenerator::linear_mesh(0.01, 0.1, 0.01);
    auto put_price_matrix_rsig = OptionPriceMatrix::compute_value_matrix(
        r_mesh, OptionPriceMatrix::ParamType::r,
        sig_mesh, OptionPriceMatrix::ParamType::sig,
        OptionPriceMatrix::GreekType::Price,
        S_mesh[10], K, T, 0.0, 0.0, b);
    
    print_value_matrix(put_price_matrix_rsig, r_mesh, sig_mesh, "r", "Sigma", "Put Price");

    // Part 2a: Greeks verification
    std::cout << "\n=== Part 2a: Implement gamma formula ===\n";
    K = 100; double S = 105; T = 0.5; r = 0.1; b = 0; sig = 0.36;
    double call_delta = Greeks::Calculate_call_delta(S,K,T,r,sig,b);
    double put_delta = Greeks::Calculate_put_delta(S,K,T,r,sig,b);
    double gamma = Greeks::Calculate_gamma(S,K,T,r,sig,b);
    std::cout << "Call delta calculated: " << call_delta << ". Should be 0.5946" << std::endl;
    std::cout << "Put delta calculated: " << put_delta << ". Should be -0.3566" << std::endl;
    std::cout << "Gamma calculated: " << gamma << std::endl;

    // Part 2b: Mesh of S values for deltas
    std::cout << "\n=== Part 2b: Mesh of S Values for Deltas ===\n";
    S_mesh = MeshGenerator::linear_mesh(80.0, 120.0, 1.0);
    std::vector<double> delta_prices;
    delta_prices.reserve(S_mesh.size());

    std::cout << "Delta prices for S in [80, 120], K=100, T=0.5, r=0.1, sig=0.36:\n";
    for (double S : S_mesh) {
        call_delta = Greeks::Calculate_call_delta(S, K, T, r, sig, b);
        delta_prices.push_back(call_delta);
        std::cout << "S=" << S << ", Delta=" << call_delta << "\n";
    }

    // Part 2c: Delta and Gamma matrices
    std::cout << "\n=== Part 2c: Delta and Gamma Matrices ===\n";
    S_mesh = MeshGenerator::linear_mesh(80.0, 120.0, 5.0);
    K_mesh = MeshGenerator::linear_mesh(90.0, 110.0, 5.0);
    
    auto delta_matrix = OptionPriceMatrix::compute_value_matrix(
        S_mesh, OptionPriceMatrix::ParamType::S,
        K_mesh, OptionPriceMatrix::ParamType::K,
        OptionPriceMatrix::GreekType::CallDelta,
        0.0, 0.0, 0.5, 0.1, 0.36, 0.0);
    
    print_value_matrix(delta_matrix, S_mesh, K_mesh, "S", "K", "Call Delta");

    T_mesh = MeshGenerator::linear_mesh(0.1, 1.0, 0.1);
    auto gamma_matrix = OptionPriceMatrix::compute_value_matrix(
        S_mesh, OptionPriceMatrix::ParamType::S,
        T_mesh, OptionPriceMatrix::ParamType::T,
        OptionPriceMatrix::GreekType::Gamma,
        0.0, 100.0, 0.0, 0.1, 0.36, 0.0);
    
    print_value_matrix(gamma_matrix, S_mesh, T_mesh, "S", "T", "Gamma");

     std::cout << "\n=== Part 2d: Numerical Greeks ===\n";
    
    // Create numerical greeks calculator for call options
    NumericalGreeks::PricingFunction call_pricer = [](double S, double K, double T, 
                                                     double r, double sigma, double b) {
        return BlackScholes::call_price(S, K, T, r, sigma, b);
    };
    
    NumericalGreeks num_greeks(call_pricer);
    
    // Test different step sizes
    std::vector<double> h_values = {0.1, 0.01, 0.001, 0.0001};
    K = 100; S = 105; T = 0.5; r = 0.1; b = 0; sig = 0.36;
    
    std::cout << "\nComparing analytical and numerical Greeks:\n";
    std::cout << "S=" << S << ", K=" << K << ", T=" << T << ", r=" << r << ", sig=" << sig << "\n";
    
    // Get analytical values
    double exact_delta = Greeks::Calculate_call_delta(S, K, T, r, sig, b);
    double exact_gamma = Greeks::Calculate_gamma(S, K, T, r, sig, b);
    double exact_theta = Greeks::Calculate_theta(S, K, T, r, sig, b);
    
    std::cout << "\nAnalytical values:\n";
    std::cout << "Delta: " << exact_delta << "\n";
    std::cout << "Gamma: " << exact_gamma << "\n";
    std::cout << "Theta: " << exact_theta << "\n";
    
    // Compare with numerical approximations
    std::cout << "\nNumerical approximations:\n";
    std::cout << "h\tDelta\t\tError\t\tGamma\t\tError\t\tTheta\t\tError\n";
    for (double h : h_values) {
        num_greeks.set_step_size(h);
        
        double num_delta = num_greeks.delta(S, K, T, r, sig, b);
        double num_gamma = num_greeks.gamma(S, K, T, r, sig, b);
        double num_theta = num_greeks.theta(S, K, T, r, sig, b);
        
        std::cout << h << "\t"
                  << num_delta << "\t" << fabs(num_delta - exact_delta) << "\t"
                  << num_gamma << "\t" << fabs(num_gamma - exact_gamma) << "\t"
                  << num_theta << "\t" << fabs(num_theta - exact_theta) << "\n";
    }

    // Part 2: Numerical Greeks mesh
    std::cout << "\n=== Part 2: Numerical Greeks Mesh ===\n";
    S_mesh = MeshGenerator::linear_mesh(80.0, 120.0, 5.0);
    h_values = {0.01, 0.001};  // Test two step sizes
    
    for (double h : h_values) {
        num_greeks.set_step_size(h);
        std::cout << "\nNumerical Delta with h=" << h << ":\n";
        
        for (double S : S_mesh) {
            double num_d = num_greeks.delta(S, K, T, r, sig, b);
            double exact_d = Greeks::Calculate_call_delta(S, K, T, r, sig, b);
            
            std::cout << "S=" << S << ": Num=" << num_d 
                      << ", Exact=" << exact_d
                      << ", Error=" << fabs(num_d - exact_d) << "\n";
        }
    }

    // End of Part A
    // Start of Part B

    // Part b: Test American option pricing with given parameters
    std::cout << "\n=== Part b: American Option Validation ===\n";
    {
        K = 100.0, sig = 0.1, r = 0.1, b = 0.02, S = 110.0;
        double american_call = AmericanOption::Calculate_call(S, K, 0.0, r, sig, b); // T=0 for perpetual
        double american_put = AmericanOption::Calculate_pul(S, K, 0.0, r, sig, b);   // T=0 for perpetual
        
        std::cout << "American Call Price: " << american_call 
                  << " (Expected ~18.5035)\n";
        std::cout << "American Put Price: " << american_put 
                  << " (Expected ~3.03106)\n";
        
        // Compare with European prices
        double european_call = BlackScholes::call_price(S, K, 1.0, r, sig, b); // T=1 for comparison
        double european_put = BlackScholes::put_price(S, K, 1.0, r, sig, b);
        
        std::cout << "\nEuropean Call Price (T=1): " << european_call << "\n";
        std::cout << "European Put Price (T=1): " << european_put << "\n";
    }

    // Part c: Mesh of American option prices
    std::cout << "\n=== Part c: American Option Price Mesh ===\n";
    {
        S_mesh = MeshGenerator::linear_mesh(10.0, 50.0, 1.0);
        K = 30.0, r = 0.05, sig = 0.2, b = 0.02;
        
        std::vector<double> american_call_prices;
        std::vector<double> american_put_prices;
        american_call_prices.reserve(S_mesh.size());
        american_put_prices.reserve(S_mesh.size());
        
        std::cout << "American option prices for S in [10, 50], K=30, r=0.05, sig=0.2, b=0.02:\n";
        std::cout << "S\tCall\tPut\n";
        for (double S : S_mesh) {
            double call_price = AmericanOption::Calculate_call(S, K, 0.0, r, sig, b);
            double put_price = AmericanOption::Calculate_pul(S, K, 0.0, r, sig, b);
            
            american_call_prices.push_back(call_price);
            american_put_prices.push_back(put_price);
            
            std::cout << S << "\t" << call_price << "\t" << put_price << "\n";
        }
    }

    // Part d: American Option Price Matrix
    std::cout << "\n=== Part d: American Option Price Matrix ===\n";

    // Example: Varying S and K for American calls
    S_mesh = MeshGenerator::linear_mesh(80.0, 120.0, 5.0);
    K_mesh = MeshGenerator::linear_mesh(90.0, 110.0, 5.0);
    r = 0.1, sig = 0.2, b = 0.02;
        
    auto american_call_matrix = AmericanOptionMatrix::compute_american_matrix(
        S_mesh, OptionPriceMatrix::ParamType::S,
        K_mesh, OptionPriceMatrix::ParamType::K,
        true,  // is_call
        0.0, 0.0, 0.0, r, sig, b);  // S and K will be overridden
        
    std::cout << "\nAmerican Call Price Matrix (varying S and K):\n";
    print_value_matrix(american_call_matrix, S_mesh, K_mesh, "S", "K", "Call Price");

    // Example: Varying r and sigma for American puts
    r_mesh = MeshGenerator::linear_mesh(0.01, 0.1, 0.01);
    sig_mesh = MeshGenerator::linear_mesh(0.1, 0.5, 0.05);
    S = 100.0, K = 100.0;
        
    auto american_put_matrix = AmericanOptionMatrix::compute_american_matrix(
        r_mesh, OptionPriceMatrix::ParamType::r,
        sig_mesh, OptionPriceMatrix::ParamType::sig,
        false,  // is_put
        S, K, 0.0, 0.0, 0.0, b);  // r and sig will be overridden
        
    std::cout << "\nAmerican Put Price Matrix (varying r and sigma):\n";
    print_value_matrix(american_put_matrix, r_mesh, sig_mesh, "r", "Sigma", "Put Price");
    

    return 0;
}