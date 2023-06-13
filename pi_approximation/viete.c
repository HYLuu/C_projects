#include "mathlib.h"

static int ct = 0; // static variable for counting iteration terms

// Approximate the value of π using the Viète’s Formula
// Return the approximation of π
//
// No parameter
double pi_viete(void) {
    double x = sqrt_newton(2.0), last = 0.0, current = 0.5 * sqrt_newton(2.0); // Initialization
    while (absolute(current - last) > EPSILON) { // Approximation converge
        last = current;
        current *= 0.5 * sqrt_newton(2 + x);
        x = sqrt_newton(2 + x); // x is the variable for the a_k = sqrt(2 + a_k-1) implementation
        ct++;
    }
    return 1.0 / (current / 2);
}

// Return the number of iterations occurred in pi_viete()
//
// No parameter
int pi_viete_factors(void) {
    return ct;
}
