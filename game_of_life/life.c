#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "tsn:i:o:h"
void swap(Universe *x, Universe *y); // helper function declaration
void one_generation(Universe *u_A, Universe *u_B, uint32_t r, uint32_t c);

// Usage information of this program
// Return void
//
// *exec is the executale
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Conway's Game of Life\n"
        "\n"
        "USAGE\n"
        "   %s tsn:i:o:h\n"
        "\n"
        "OPTIONS\n"
        "    -t             Create your universe as a toroidal\n"
        "    -s             Silent - do not use animate the evolution\n"
        "    -n {number}    Number of generations [default: 100]\n"
        "    -i {file}      Input file [default: stdin]\n"
        "    -o {file}      Output file [default: stdout]\n",
        exec);
}

// Show the Game of Life evolution animation according to command-line options and input patterns
// Return 0
//
// argc is the number of arguments
// **argv is the arguments
int main(int argc, char **argv) {
    int opt = 0;
    uint32_t num = 100; // Default generation number
    bool torus = false; // Not creating universe as a toroidal if not specified
    bool silence = false; // Use animation of the evolution unless specified
    bool inp = false; // Input file not specified
    bool out = false; // Output to file not specified
    char *input; // Input file name
    char *output; // Output file name
    FILE *finptr; // Input file pointer
    FILE *foutptr; // Output file pointer
    uint32_t r = 0; // Variable for read-in row number
    uint32_t c = 0; // Variable for read-in column number

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // Parse the arguments
        switch (opt) {
        case 't': torus = true; break;
        case 's': silence = true; break;
        case 'n': num = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'i':
            inp = true;
            input = optarg;
            break;
        case 'o':
            out = true;
            output = optarg;
            break;
        case 'h': usage(argv[0]); exit(1); // Help information
        default: usage(argv[0]); return EXIT_FAILURE; // Default setting if get an unknown argument
        }
    }

    // Input file not specified so read from stdin
    if (inp == false) {
        // Read the number of rows and columns of the universe
        fscanf(stdin, "%" SCNd32 " %" SCNd32 " ", &r, &c);
    } else { // Read from file
        if ((finptr = fopen(input, "r")) == NULL) { // fopen returns null if the file doesn't exist
            perror("Error opening file.");
            exit(1);
        } else {
            // Read the number of rows and columns of the universe
            fscanf(finptr, "%" SCNd32 "%" SCNd32 " ", &r, &c);
        }
    }

    // Create two universes using the dimensions obtained from input
    Universe *uni_A = uv_create(r, c, torus);
    Universe *uni_B = uv_create(r, c, torus);

    // Populate universe A with the remainder of the input
    if (inp == false) { // Read from stdin
        bool populated = uv_populate(uni_A, stdin);
        if (populated == false) {
            fprintf(stderr, "Malformed input.\n");
            exit(1);
        }
    } else { // Read from file
        bool populated = uv_populate(uni_A, finptr);
        if (populated == false) {
            fprintf(stderr, "Malformed input.\n");
            exit(1);
        }
    }

    fclose(finptr);

    uint32_t r_A = uv_rows(uni_A); // Get row numbers of universe A
    uint32_t c_A = uv_cols(uni_A); // Get column numbers of universe A

    // When ncurses not silenced, display animation
    if (silence == false) {
        initscr(); // Initialize the screen.
        curs_set(FALSE); // Hide the cursor.

        //For each generation up to the set number of generations
        for (uint32_t k = 0; k < num; k += 1) {
            clear(); // Clear the window.
            // Display universe A
            for (uint32_t i = 0; i < r_A; i += 1) {
                for (uint32_t j = 0; j < c_A; j += 1) {
                    if (uv_get_cell(uni_A, i, j) == true) {
                        mvprintw(i, j, "o");
                    } else {
                        mvprintw(i, j, ".");
                    }
                }
            }
            refresh(); // Refresh the window.
            usleep(50000); // Sleep for 50000 microseconds.

            // Perform a generation
            one_generation(uni_A, uni_B, r_A, c_A);

            // Swap the universes
            swap(uni_A, uni_B);
        }
        endwin();
    } else {
        //For each generation up to the set number of generations
        for (uint32_t k = 0; k < num; k += 1) {

            // Perform a generation
            one_generation(uni_A, uni_B, r_A, c_A);

            // Swap the universes
            swap(uni_A, uni_B);
        }
    }

    // Print the final state of the universe
    if (out == false) {
        uv_print(uni_A, stdout);
    } else {
        foutptr = fopen(output, "w"); // output pointer points to file for writing
        uv_print(uni_A, foutptr);
    }

    // Delete universes -- free the allocated memory
    uv_delete(uni_A);
    uv_delete(uni_B);

    return 0;
}
