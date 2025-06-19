#pragma once
#include <cmath>
#include <boost/math/distributions/normal.hpp>

namespace AmericanOption {
    inline double y1 (double r, double sig, double b) {
        return 0.5 - b / (sig * sig) + 
                sqrt( (b / (sig * sig) - 0.5) * (b / (sig * sig) - 0.5) + 2 * r / sig / sig);
    }

    inline double y2 (double r, double sig, double b) {
        return 0.5 - b / (sig * sig) -
                sqrt( (b / (sig * sig) - 0.5) * (b / (sig * sig) - 0.5) + 2 * r / sig / sig);
    }

    inline double Calculate_call (double S, double K, double T, double r, double sig, double b){
        double y_1 = y1(r,sig,b);
        return K / (y_1 - 1) * pow(((y_1 - 1)/y_1 * S / K),y_1);

    }

    inline double Calculate_pul (double S, double K, double T, double r, double sig, double b){
        double y_2 = y2(r,sig,b);
        return K / (1 - y_2) * pow(((y_2 - 1)/y_2 * S / K),y_2);

    }
}