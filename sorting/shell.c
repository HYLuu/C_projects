#include "gaps.h"
#include "stats.h"

// Perfor Shell Sort
// Return void
//
// *stats is the struct to store the moves and compares the algorithm has performed
// *arr is the array to sort
// length is the length of the array
void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    for (uint32_t k = 0; k < GAPS; k += 1) {
        for (uint32_t i = gaps[k]; i < length; i += 1) { // For each gap length
            uint32_t j = i;
            uint32_t temp = arr[i];
            while (j >= gaps[k] && (cmp(stats, arr[j - gaps[k]], temp) == 1)) {
                arr[j] = arr[j - gaps[k]];
                move(stats, 1);
                j -= gaps[k];
            }
            arr[j] = temp;
            move(stats, 1);
        }
    }
}
