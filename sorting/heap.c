#include "stats.h"

#include <stdbool.h>

// Determine the index of the biggest child
// Return integer
//
// *stats is the struct to store the moves and compares the algorithm has performed
// *A is the array to sort
// first is the first index to choose the biggest child
// last is the last index to choose the biggest child
int max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if ((right <= last) && (cmp(stats, A[right - 1], A[left - 1]) == 1)) {
        return right;
    }
    return left;
}

// To fix the heap
// Return void
//
// *stats is the struct to store the moves and compares the algorithm has performed
// *A is the array to sort
// first is the first index of the array for fixing the heap
// last is the last index of the array for fixing the heap
void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last); // Determine the index of the biggest child

    while ((mother <= (last / 2)) && (!found)) {
        if (cmp(stats, A[great - 1], A[mother - 1]) == 1) {
            swap(stats, &A[great - 1], &A[mother - 1]);
            mother = great;
            great = max_child(stats, A, mother, last);
        } else {
            found = true;
        }
    }
}

// Build a heap
// Return void
//
// *stats is the struct to store the moves and compares the algorithm has performed
// *A is the array to sort
// first is the first index of the array to build heap
// last is the last index of the array to build heap
void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    for (uint32_t i = last / 2; i > (first - 1); i -= 1) { // Start from the middle and go reverse
        fix_heap(stats, A, i, last);
    }
}

// Perfor Heap Sort
// Return void
//
// *stats is the struct to store the moves and compares the algorithm has performed
// *A is the array to sort
// n is the length of the array
void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t first = 1;
    uint32_t last = n;
    build_heap(stats, A, first, last); // Build the heap
    for (uint32_t i = last; i >= first; i -= 1) { // Reverse
        swap(stats, &A[first - 1], &A[i - 1]);
        fix_heap(stats, A, first, i - 1);
    }
}
