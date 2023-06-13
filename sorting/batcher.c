#include "stats.h"

// To get bit length
// Return unsigned 32 bit integer
//
// x is the number to get bit length from
uint32_t bit_length(uint32_t x) {
    uint32_t ct = 0;
    while (x) {
        ct += 1;
        x >>= 1; // Shift until x == 0
    }
    return ct;
}

// Compare the values traveling along the two wires and swap the values if they’re out of order
// Return void
//
// *stats is the struct to store the moves and compares the algorithm has performed
// *A is the array to sort
// x and y are the indices of the values to compare
void comparator(Stats *stats, uint32_t *A, int x, int y) {
    if (cmp(stats, A[x], A[y]) == 1) {
        swap(stats, &A[x], &A[y]);
    }
}

// Perfor Batcher Sort
// Return void
//
// *stats is the struct to store the moves and compares the algorithm has performed
// *A is the array to sort
// n is the length of the array
void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    if (n == 0) { // if no element to sort
        return;
    }

    uint32_t t = bit_length(n);
    uint32_t p = 1 << (t - 1);

    while (p > 0) {
        uint32_t q = 1 << (t - 1);
        uint32_t r = 0;
        uint32_t d = p;
        while (d > 0) {
            for (uint32_t i = 0; i < n - d; i += 1) {
                if ((i & p) == r) {
                    comparator(stats, A, i, i + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
