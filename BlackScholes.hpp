#pragma once
#include <cmath>
#include <boost/math/distributions/normal.hpp>

namespace BlackScholes {

    // Calculate d1 term in Black-Scholes
    inline double calculate_d1(double S, double K, double T, double r, double sig, double b = 0.0) {
        return (log(S / K) + (b + sig * sig / 2.0) * T) / (sig * sqrt(T));
    }

    // Calculate d2 term in Black-Scholes
    inline double calculate_d2(double d1, double sig, double T) {
        return d1 - sig * sqrt(T);
    }

    // Calculate call price
    inline double call_price(double S, double K, double T, double r, double sig, double b = 0.0) {
        boost::math::normal_distribution<> normal(0.0, 1.0);
        double d1 = calculate_d1(S, K, T, r, sig, b);
        double d2 = calculate_d2(d1, sig, T);
        return S * exp((b - r) * T) * boost::math::cdf(normal, d1) - K * exp(-r * T) * boost::math::cdf(normal, d2);
    }

    // Calculate put price
    inline double put_price(double S, double K, double T, double r, double sig, double b = 0.0) {
        boost::math::normal_distribution<> normal(0.0, 1.0);
        double d1 = calculate_d1(S, K, T, r, sig, b);
        double d2 = calculate_d2(d1, sig, T);
        return K * exp(-r * T) * boost::math::cdf(normal, -d2) - S * exp((b - r) * T) * boost::math::cdf(normal, -d1);
    }
}