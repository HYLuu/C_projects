#include "stats.h"

#include <stdbool.h>

// Partitions arrays into two sub-arrays by selecting an element from the array and designating it as a pivot
// Return unsigned 32 bit integer
//
// *stats is the struct to store the moves and compares the algorithm has performed
// *A is the array to sort
// lo is the first index of the array to partition
// hi is the last index of the array to partition
uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t i = lo - 1;
    for (uint32_t j = lo; j < hi; j += 1) {
        if (cmp(stats, A[hi - 1], A[j - 1]) == 1) {
            i += 1;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }
    swap(stats, &A[i], &A[hi - 1]);
    return i + 1;
}

// Recursive helper function
// *stats is the struct to store the moves and compares the algorithm has performed
// *A is the array to sort
// lo is the first index of the array to sort
// hi is the last index of the array to sort
void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    if (lo < hi) {
        int p = partition(stats, A, lo, hi);
        quick_sorter(stats, A, lo, p - 1);
        quick_sorter(stats, A, p + 1, hi);
    }
}

// Perfor Quick Sort
// Return void
//
// *stats is the struct to store the moves and compares the algorithm has performed
// *A is the array to sort
// n is the length of the array
void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
