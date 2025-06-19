#pragma once
#include <cmath>
#include <stdexcept>

namespace PutCallParity {

    // Calculate call price from put price
    inline double call_from_put(double P, double S, double K, double T, double r) {
        return P + S - K * exp(-r * T);
    }

    // Calculate put price from call price
    inline double put_from_call(double C, double S, double K, double T, double r) {
        return C - S + K * exp(-r * T);
    }

    // Check if call/put prices satisfy parity
    inline bool satisfies_parity(double C, double P, double S, double K, double T, double r, double tol = 1e-6) {
        double lhs = C - P;
        double rhs = S - K * exp(-r * T);
        return abs(lhs - rhs) < tol;
    }
}