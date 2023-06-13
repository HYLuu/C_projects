#include "mathlib.h"

static int ct = 0; // static variable for counting iteration terms

// Approximate the value of π using the Madhava Series
// Return the approximation of π
//
// No parameter
double pi_madhava(void) {
    double x = 1.0, y = 1.0, last = 0.0, current = 1.0; // Initialization
    while (absolute(current - last) > EPSILON) { // Approximation converge
        last = current;
        current += 1 / (2 * x + 1) * (y / -3);
        y = y / -3; // y is the variable for the (-3)^-k implementation
        x++;
        ct++;
    }
    return current * sqrt_newton(12.0);
}

// Return the number of iterations occurred in pi_madhava()
//
// No parameter
int pi_madhava_terms(void) {
    return ct;
}
