# Options Pricing Library

C++ implementation of financial option pricing models including:
- Black-Scholes (European options)
- Perpetual American options
- Greeks calculation
- Numerical approximations

## Features
- Matrix pricing for parameter sensitivity analysis
- Mesh generation for underlying prices
- Exact formulas and numerical approximations
- Comprehensive test cases

## Usage
```cpp
#include "AmericanOption.hpp"

double price = AmericanOption::Calculate_call(S, K, T, r, sig, b);
```

## Build Instructions
```bash
mkdir build && cd build
cmake ..
make
./OptionsPricing
```

## Dependencies
- C++17
- Boost Math (for normal distribution)