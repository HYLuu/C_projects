#include "mathlib.h"

static int ct = 0; // static variable for counting iteration terms

// Approximate the square root value of input using the Newton’s method
// Return the approximation of π
//
// x is a floating point number
double sqrt_newton(double x) {
    double s = 1.0, y = 1.0, z = 0.0; // s is scaling factor
    while (x > 4) { // Normalize to [0, 4) by reducing the exponent
        x /= 4.0; // Every 4 going in results in 2 going out since √4 = 2
        s *= 2.0; // Store √4 = 2
    }
    while (absolute(y - z) > EPSILON) {
        z = y;
        y = 0.5 * (y + x / y);
        ct++;
    }
    return s * y; // Rescale back to the original range
}

// Return the number of iterations occurred in sqrt_newton()
//
// No parameter
int sqrt_newton_iters(void) {
    return ct;
}
