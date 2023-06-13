#include "batcher.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "Hasbhqn:p:r:"

typedef enum {
    SHELL,
    QUICK,
    HEAP,
    BATCHER,
    HELP,
    END
} SORTS; // enum array to represent each sorting method

// Fill an array with bit-masked random integers
// Return 0
//
// *Array is an array to fill with generated numbers
// length is the length of the given array
// seed is the random seed used for generating certain set of random numbers
int get_randarr(uint32_t *Array, uint32_t length, uint32_t seed) {
    srandom(seed); // Set seed
    uint32_t mask = ~((1 << 31) | (1 << 30)); // Mask for bit-masking to 30 bits
    for (uint32_t i = 0; i < length; i += 1) {
        Array[i] = random();
        Array[i] = Array[i] & mask;
    }
    return 0;
}

// Usage information of this program
// Return void
//
// *exec is the executale
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   A collection of comparison-based sorting algorithms.\n"
        "\n"
        "USAGE\n"
        "   %s [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]\n"
        "\n"
        "OPTIONS\n"
        "   -H              Display program help and usage.\n"
        "   -a              Enable all sorts.\n"
        "   -s              Enable Shell Sort.\n"
        "   -b              Enable Batcher Sort.\n"
        "   -h              Enable Heap Sort.\n"
        "   -q              Enable Quick Sort.\n"
        "   -n length       Specify number of array elements (default: 100).\n"
        "   -p elements     Specify number of elements to print (default: 100).\n"
        "   -r seed         Specify random seed (default: 13371453).\n",
        exec);
}

// Print results of sorting algorithms according to command-line options
// Return 0
//
// argc is the number of arguments
// **argv is the arguments
int main(int argc, char **argv) {
    void (*func_ptr[4])(Stats * stats, uint32_t * A, uint32_t n) = { shell_sort, quick_sort,
        heap_sort, batcher_sort }; // An array of function pointers for four sorting methods
    int opt = 0;
    Set set_vec = 0; // Set vector to store and determine the input command-line options
    uint32_t length = 100; // Default array length
    uint32_t elements = 100; // Default number of elements to print
    uint32_t seed = 13371453; // Default seed
    Stats arr_stats[4] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }; // An array of Stats struct
    char arr_name[4][13] = { "Shell Sort", "Quick Sort", "Heap Sort",
        "Batcher Sort" }; // An array of the names of sorting methods

    // Print out usage information if there is no argument
    if (argc == 1) {
        fprintf(stderr, "Select at least one sort to perform.\n");
        usage(argv[0]);
    }

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // Parse the arguments
        switch (opt) {
        case 's':
            set_vec = set_insert(set_vec, SHELL);
            break; // Turn on the bit for Shell Sort in the vector
        case 'q': set_vec = set_insert(set_vec, QUICK); break;
        case 'h': set_vec = set_insert(set_vec, HEAP); break;
        case 'b': set_vec = set_insert(set_vec, BATCHER); break;
        case 'a':
            set_vec = set_insert(set_vec, SHELL);
            set_vec = set_insert(set_vec, BATCHER);
            set_vec = set_insert(set_vec, HEAP);
            set_vec = set_insert(set_vec, QUICK);
            break;
        case 'H': set_vec = set_insert(set_vec, HELP); break;
        case 'n':
            length = (uint32_t) strtoul(optarg, NULL, 10);
            break; // Change input value to usable array length
        case 'p': elements = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'r': seed = (uint32_t) strtoul(optarg, NULL, 10); break;
        default: usage(argv[0]); return EXIT_FAILURE; // Default setting if get an unknown argument
        }
    }

    uint32_t *Array = (uint32_t *) calloc(
        length, sizeof(uint32_t)); // Allocate memory for Array according to input length
    while (true) {
        if (set_member(set_vec, HELP)) {
            usage(argv[0]);
            break;
        } else if (set_member(set_vec, SHELL) && set_member(set_vec, QUICK)
                   && set_member(set_vec, HEAP) && set_member(set_vec, BATCHER)) { // option "-a"
            for (SORTS s = SHELL; s <= BATCHER; s += 1) { // Implement every sort
                get_randarr(Array, length, seed); // Fill the array
                func_ptr[s](&arr_stats[s], Array, length); // Perform sorting
                printf("%s, %d elements, %" PRIu64 " moves, %" PRIu64 " compares.\n", arr_name[s],
                    length, arr_stats[s].moves, arr_stats[s].compares);
                if (elements >= length) { // Printing format
                    for (uint32_t i = 0; i < length; i += 1) {
                        if ((i + 1) % 5 != 0) {
                            if (i == (length - 1)) {
                                printf("%13" PRIu32 "\n", Array[i]);
                            } else {
                                printf("%13" PRIu32, Array[i]);
                            }
                        } else {
                            printf("%13" PRIu32 "\n", Array[i]);
                        }
                    }
                } else if (elements < length) {
                    for (uint32_t i = 0; i < elements; i += 1) {
                        if ((i + 1) % 5 != 0) {
                            if (i == (elements - 1)) {
                                printf("%13" PRIu32 "\n", Array[i]);
                            } else {
                                printf("%13" PRIu32, Array[i]);
                            }
                        } else {
                            printf("%13" PRIu32 "\n", Array[i]);
                        }
                    }
                }
            }
            break; // Not to run the same sort twice
        } else {
            for (SORTS s = SHELL; s <= BATCHER; s += 1) {
                if (set_member(set_vec, s)) { // Check which sort to run
                    get_randarr(Array, length, seed);
                    func_ptr[s](&arr_stats[s], Array, length);
                    printf("%s, %d elements, %" PRIu64 " moves, %" PRIu64 " compares.\n",
                        arr_name[s], length, arr_stats[s].moves, arr_stats[s].compares);
                    if (elements >= length) {
                        for (uint32_t i = 0; i < length; i += 1) {
                            if ((i + 1) % 5 != 0) {
                                if (i == (length - 1)) {
                                    printf("%13" PRIu32 "\n", Array[i]);
                                } else {
                                    printf("%13" PRIu32, Array[i]);
                                }
                            } else {
                                printf("%13" PRIu32 "\n", Array[i]);
                            }
                        }
                    } else if (elements < length) {
                        for (uint32_t i = 0; i < elements; i += 1) {
                            if ((i + 1) % 5 != 0) {
                                if (i == (elements - 1)) {
                                    printf("%13" PRIu32 "\n", Array[i]);
                                } else {
                                    printf("%13" PRIu32, Array[i]);
                                }
                            } else {
                                printf("%13" PRIu32 "\n", Array[i]);
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    free(Array); // Free allocated memory
    return 0;
}
