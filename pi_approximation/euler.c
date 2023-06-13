#include "mathlib.h"

static int ct = 0; // static variable for counting iteration terms

// Approximate the value of π using Euler’s Solution
// Return the approximation of π
//
// No parameter
double pi_euler(void) {
    double x = 2.0, last = 0.0, current = 1.0; // // Initialization
    while (absolute(current - last) > EPSILON) { // Approximation converge
        last = current;
        current += 1 / (x * x);
        x++;
        ct++;
    }
    return sqrt_newton(current * 6);
}

// Return the number of iterations occurred in pi_euler()
//
// No parameter
int pi_euler_terms(void) {
    return ct;
}
