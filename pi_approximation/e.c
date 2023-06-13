#include "mathlib.h"

static int ct = 0;

double e(void) {
    double x = 1.0, y = 1.0, last = 0.0, current = 1.0;
    while (absolute(current - last) > EPSILON) {
        last = current;
        current += 1 / (x * y);
        y = x * y;
        x++;
        ct++;
    }
    return current;
}

int e_terms() {
    return ct;
}
