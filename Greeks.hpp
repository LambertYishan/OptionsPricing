#pragma once
#include <cmath>
#include <boost/math/distributions/normal.hpp>

namespace Greeks {
    inline double calculate_d1(double S, double K, double T, double r, double sig, double b = 0.0) {
        return (log(S / K) + (b + sig * sig / 2.0) * T) / (sig * sqrt(T));
    }

    inline double calculate_d2(double d1, double sig, double T) {
        return d1 - sig * sqrt(T);
    }

    inline double Calculate_call_delta(double S, double K, double T, double r, double sig, double b = 0.0) {
        double d1 = calculate_d1(S,K,T,r,sig,b);
        boost::math::normal_distribution <> normal(0,1);
        return boost::math::cdf(normal, d1) * exp((b - r)*T);
    }

    inline double Calculate_put_delta(double S, double K, double T, double r, double sig, double b = 0.0) {
        double d1 = calculate_d1(S,K,T,r,sig,b);
        boost::math::normal_distribution <> normal(0,1);
        return (boost::math::cdf(normal, d1) - 1) * exp((b - r)*T);
    }

    inline double Calculate_gamma (double S, double K, double T, double r, double sig, double b = 0.0) {
        double d1 = calculate_d1(S,K,T,r,sig,b);
        boost::math::normal_distribution <> normal(0,1);
        return boost::math::pdf(normal, d1) * exp((b - r)*T) / (S * sig * sqrt(T));
    }

    inline double Calculate_vega (double S, double K, double T, double r, double sig, double b = 0.0) {
        double d1 = calculate_d1(S,K,T,r,sig,b);
        double gamma = Calculate_gamma(S,K,T,r,sig,b);
        return gamma * S * S * T;
    }

    inline double Calculate_theta (double S, double K, double T, double r, double sig, double b = 0.0) {
        double d1 = calculate_d1(S,K,T,r,sig,b);
        double d2 = calculate_d2(d1, sig, T);
        boost::math::normal_distribution <> normal(0,1);
        double vega = Calculate_vega(S,K,T,r,sig,b);
        double delta = Calculate_call_delta(S,K,T,r,sig,b);
        return -(vega * sig) / (2 * T) -
                (b - r) * S * delta - (r * K * exp(-r * T) * boost::math::cdf(normal, d2));
    }

}