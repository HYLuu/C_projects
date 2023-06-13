#include "mathlib.h"

static int ct = 0; // static variable for counting iteration terms

// Approximate the value of π using the Bailey-Borwein-Plouffe formula
// Return the approximation of π
//
// No parameter
double pi_bbp(void) {
    double x = 1.0, y = 1.0, last = 0.0,
           current = (4.0 - (1.0 / 2) - (1.0 / 5) - (1.0 / 6)); // Initialization
    while (absolute(current - last) > EPSILON) { // Approximation converge
        last = current;
        current += y / 16 * 4 / (8 * x + 1) - y / 16 * 2 / (8 * x + 4) - y / 16 * 1 / (8 * x + 5)
                   - y / 16 * 1 / (8 * x + 6); // y is the variable for the (16)^-k implementation
        y = y / 16; // (16)^-k
        x++;
        ct++;
    }
    return current;
}

// Return the number of iterations occurred in pi_bbp()
//
// No parameter
int pi_bbp_terms(void) {
    return ct;
}
