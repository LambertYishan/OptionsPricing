#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <iomanip>
#include "BlackScholes.hpp"
#include "Greeks.hpp"
#include "PutCallParity.hpp"
#include "MeshGenerator.hpp"
#include "BatchPricer.hpp"

class OptionPriceMatrix {
public:
    enum class ParamType { S, K, T, r, sig, b };
    enum class GreekType { Price, CallDelta, PutDelta, Gamma };
    
    using ParamVector = std::vector<double>;
    using PriceMatrix = std::vector<std::vector<double>>;
    
    // Compute prices or Greeks varying one parameter
    static std::vector<double> compute_values(
        const ParamVector& param_values,
        ParamType param_type,
        GreekType greek_type,
        double S, double K, double T, double r, double sig, double b)
    {
        std::vector<double> values;
        values.reserve(param_values.size());
        
        for (double param : param_values) {
            auto args = get_parameters(param, param_type, S, K, T, r, sig, b);
            values.push_back(calculate_greek(greek_type, args[0], args[1], args[2], 
                            args[3], args[4], args[5]));
        }
        
        return values;
    }
    
    // Compute price/Greek matrix varying two parameters
    static PriceMatrix compute_value_matrix(
        const ParamVector& param1_values, ParamType param1_type,
        const ParamVector& param2_values, ParamType param2_type,
        GreekType greek_type,
        double S, double K, double T, double r, double sig, double b)
    {
        PriceMatrix matrix;
        matrix.reserve(param1_values.size());
        
        for (double param1 : param1_values) {
            std::vector<double> row;
            row.reserve(param2_values.size());
            
            for (double param2 : param2_values) {
                std::map<ParamType, double> params = {
                    {ParamType::S, S}, {ParamType::K, K}, 
                    {ParamType::T, T}, {ParamType::r, r},
                    {ParamType::sig, sig}, {ParamType::b, b}
                };
                
                params[param1_type] = param1;
                params[param2_type] = param2;
                
                row.push_back(calculate_greek(
                    greek_type,
                    params[ParamType::S], params[ParamType::K], params[ParamType::T],
                    params[ParamType::r], params[ParamType::sig], params[ParamType::b]));
            }
            
            matrix.push_back(row);
        }
        
        return matrix;
    }

private:
    static double calculate_greek(GreekType greek_type, 
                                double S, double K, double T, 
                                double r, double sig, double b) {
        switch(greek_type) {
            case GreekType::Price:
                return BlackScholes::call_price(S, K, T, r, sig, b);
            case GreekType::CallDelta:
                return Greeks::Calculate_call_delta(S, K, T, r, sig, b);
            case GreekType::PutDelta:
                return Greeks::Calculate_put_delta(S, K, T, r, sig, b);
            case GreekType::Gamma:
                return Greeks::Calculate_gamma(S, K, T, r, sig, b);
            default:
                throw std::invalid_argument("Unknown Greek type");
        }
    }

    static std::array<double, 6> get_parameters(
        double varying_param, ParamType param_type,
        double S, double K, double T, double r, double sig, double b)
    {
        std::array<double, 6> params = {S, K, T, r, sig, b};
        
        switch (param_type) {
            case ParamType::S: params[0] = varying_param; break;
            case ParamType::K: params[1] = varying_param; break;
            case ParamType::T: params[2] = varying_param; break;
            case ParamType::r: params[3] = varying_param; break;
            case ParamType::sig: params[4] = varying_param; break;
            case ParamType::b: params[5] = varying_param; break;
        }
        
        return params;
    }
};

// Updated print function to handle different value types
void print_value_matrix(
    const OptionPriceMatrix::PriceMatrix& matrix,
    const std::vector<double>& row_params,
    const std::vector<double>& col_params,
    const std::string& row_label,
    const std::string& col_label,
    const std::string& value_type = "Price")
{
    std::cout << "\n" << value_type << " matrix varying " << row_label << " and " << col_label << ":\n";
    std::cout << row_label << "\\" << col_label << "\t";
    
    // Print header
    for (double col_val : col_params) {
        std::cout << std::fixed << std::setprecision(4) << std::setw(8) << col_val << "\t";
    }
    std::cout << "\n";
    
    // Print rows
    for (size_t i = 0; i < matrix.size(); ++i) {
        std::cout << std::fixed << std::setprecision(2) << std::setw(8) << row_params[i] << "\t";
        for (double val : matrix[i]) {
            std::cout << std::fixed << std::setprecision(4) << std::setw(8) << val << "\t";
        }
        std::cout << "\n";
    }
}