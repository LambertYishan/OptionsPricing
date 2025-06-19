#pragma once
#include "OptionPricingMatrix.hpp"
#include "AmericanOptions.hpp"


// Extend OptionPriceMatrix to handle American options
class AmericanOptionMatrix : public OptionPriceMatrix {
    public:
        static PriceMatrix compute_american_matrix(
        const ParamVector& param1_values, ParamType param1_type,
        const ParamVector& param2_values, ParamType param2_type,
        bool is_call,
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
                        
                    if (is_call) {
                        row.push_back(AmericanOption::Calculate_call(
                            params[ParamType::S], params[ParamType::K], params[ParamType::T],
                            params[ParamType::r], params[ParamType::sig], params[ParamType::b]));
                    } else {
                        row.push_back(AmericanOption::Calculate_pul(
                            params[ParamType::S], params[ParamType::K], params[ParamType::T],
                            params[ParamType::r], params[ParamType::sig], params[ParamType::b]));
                    }
                }
                matrix.push_back(row);
            }
            return matrix;
        }
    };