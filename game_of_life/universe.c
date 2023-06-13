#include "universe.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Declaration of type Universe details
struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

// Check if the input numbers of rows and columns are in the defined range of established Universe
// Return 1 if input are within the range, 0 if not
//
// rows and cols are the range of the Universe instance
// r and c are the inputs to be checked
int in_bound(uint32_t rows, uint32_t cols, uint32_t r, uint32_t c) {
    if ((r + 1) <= rows && (c + 1) <= cols) {
        return 1;
    } else {
        return 0;
    }
}

// Swap universes
// Return void
//
// *x and *y are univer pointers to swap
void swap(Universe *x, Universe *y) {
    Universe tmp = *x;
    *x = *y;
    *y = tmp;
}

// Get modulo result
// Return a int32_t number
//
// a an b are 32 bit integers
int32_t mod(int32_t a, int32_t b) { // code modified from dewang
    int32_t n = a % b;
    return n < 0 ? n + b : n;
}

// Create a Universe
// Return a pointer to the Universe struct instance
//
// rows and cols are the row & column numbers of the Universe
// toroidal specifies if the Universe is toroidal
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u; // Universe pointer
    u = (Universe *) calloc(1, sizeof(Universe)); // Allocate memory for the Universe
    assert(u != NULL); // The Universe is created

    // Create the grid
    bool **grid = (bool **) calloc(rows, sizeof(bool *)); // Allocate a column of pointers to rows
    for (uint32_t r = 0; r < rows; r += 1) {
        grid[r] = (bool *) calloc(cols, sizeof(bool)); // Allocate rows
    }

    // Define the struct instance
    u->rows = rows;
    u->cols = cols;
    u->grid = grid;
    u->toroidal = toroidal;

    return u;
}

// Destructor function of a Universe
// Return void
//
// *u is a pointer to a Universe
void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i += 1) {
        free(u->grid[i]); // Free the memory for rows first
    }
    free(u->grid); // Then free the column of pointers to rows
    free(u); // Free the memory allocated for the Universe struct
}

// Get the number of rows of the specified Universe
// Return the number of rows
//
// *u is a pointer to a Universe
uint32_t uv_rows(Universe *u) {
    return u->rows;
}

// Get the number of columns of the specified Universe
// Return the number of columns
//
// *u is a pointer to a Universe
uint32_t uv_cols(Universe *u) {
    return u->cols;
}

// Mark the cell at (r, c) in Universe *u as alive
// Return void
//
// r and c are the row/column index of the cell to be marked
// *u is a pointer to a Universe
void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if ((r + 1) <= u->rows && (c + 1) <= u->cols) { // indicies in the range of Universe
        u->grid[r][c] = true;
    }
}

// Mark the cell at (r, c) in Universe *u as dead
// Return void
//
// r and c are the row/column index of the cell to be marked
// *u is a pointer to a Universe
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if ((r + 1) <= u->rows && (c + 1) <= u->cols) { // indicies in the range of Universe
        u->grid[r][c] = false;
    }
}

// Check whether a cell is live or dead
// Return the value of the cell at (r, c)
//
// r and c are the row/column index of the cell to be checked
// *u is a pointer to a Universe
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if ((r + 1) <= u->rows && (c + 1) <= u->cols) {
        return u->grid[r][c];
    } else {
        return false; // Return false if row and column are out-of-bounds
    }
}

// Populate the Universe with row-column pairs read in from infile
// Return true if the universe is successfully populated, false for ailure to populate
//
// *infile is the input to read from
// *u is a pointer to a Universe to be populated
bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r = 0;
    uint32_t c = 0;

    while (true) { // Read every lines of inputs
        // Read from infile and place row-column pairs into r and c, respectively
        int input = fscanf(infile, "%" SCNu32 " %" SCNu32 "", &r, &c);
        // Exactly two elements are read and they are in the range of the Universe *u
        if (input == 2 && in_bound(u->rows, u->cols, r, c) == 1) {
            u->grid[r][c] = true; // Mark the cell as live
        } else if (in_bound(u->rows, u->cols, r, c) == 0) { // out-of bounds
            return false;
        } else if (input == EOF) { // End of file
            return true;
            break;
        } else if (input != 2 && input != EOF) { // Malformated input
            return false;
        }
    }
}

// Check the number of live neighbors of a given cell at (r, c)
// Return the number of live neighbors adjacent to the cell at (r, c)
//
// r and c are the row/column index of the cell to be checked
// *u is a pointer to a Universe
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    if (u->toroidal == false) { // Universe is flat
        uint32_t live = 0;
        // unsigned -> signed
        int32_t r_int = (int32_t) r;
        int32_t c_int = (int32_t) c;
        // Loop through nine cells in a grid
        for (int32_t i = r_int - 1; i < r_int + 2; i += 1) {
            for (int32_t j = c_int - 1; j < c_int + 2; j += 1) {
                if ((i + 1) <= (int32_t) u->rows && (j + 1) <= (int32_t) u->cols && i >= 0 && j >= 0
                    && u->grid[i][j] == true) { // (i, j) in bound and the cell is live
                    live += 1;
                }
            }
        }
        // live - 1 if the middle cell is counted
        if (u->grid[r][c] == true) {
            return live - 1;
        } else {
            return live;
        }
    } else { // Universe is toroidal
        uint32_t live = 0;
        // unsigned -> signed
        int32_t r_int = (int32_t) r;
        int32_t c_int = (int32_t) c;
        for (int32_t i = r_int - 1; i < r_int + 2; i += 1) {
            for (int32_t j = c_int - 1; j < c_int + 2; j += 1) {
                // Apply modulo index if (i, j) is out-of-bounds
                if ((i + 1) > (int32_t) u->rows || i < 0 || (j + 1) > (int32_t) u->cols || j < 0) {
                    if (u->grid[mod(i, (int32_t) u->rows)][mod(j, (int32_t) u->cols)] == true) {
                        live += 1;
                    }
                } else {
                    if (u->grid[i][j] == true) {
                        live += 1;
                    }
                }
            }
        }
        // live - 1 if the middle cell is counted
        if (u->grid[r][c] == true) {
            return live - 1;
        } else {
            return live;
        }
    }
}

// Prints out the universe to outfile
// Return void
//
// *outfile is where to write
// *u is a pointer to a Universe
void uv_print(Universe *u, FILE *outfile) {
    // For each cell in the Universe
    for (uint32_t i = 0; i < u->rows; i += 1) {
        for (uint32_t j = 0; j < u->cols; j += 1) {
            if ((j + 1) == u->cols) {
                if (u->grid[i][j] == true) {
                    fprintf(outfile, "o\n");
                } else {
                    fprintf(outfile, ".\n");
                }
            } else {
                if (u->grid[i][j] == true) {
                    fprintf(outfile, "o");
                } else {
                    fprintf(outfile, ".");
                }
            }
        }
    }
}

// Perform a generation
// Return void
//
// u_A and u_B are the current state and next state of the universe, respectively
// r and c are the row and column numbers of the universes
void one_generation(Universe *u_A, Universe *u_B, uint32_t r, uint32_t c) {
    for (uint32_t i = 0; i < r; i += 1) {
        for (uint32_t j = 0; j < c; j += 1) {
            //Any live cell with two or three live neighbors survives.
            if (uv_get_cell(u_A, i, j) == true && uv_census(u_A, i, j) == 2) {
                uv_live_cell(u_B, i, j);
            } else if (uv_get_cell(u_A, i, j) == true && uv_census(u_A, i, j) == 3) {
                uv_live_cell(u_B, i, j);
            }
            // Any dead cell with exactly three live neighbors becomes a live cell.
            else if (uv_get_cell(u_A, i, j) == false && uv_census(u_A, i, j) == 3) {
                uv_live_cell(u_B, i, j);
            }
            // All other cells die.
            else {
                uv_dead_cell(u_B, i, j);
            }
        }
    }
}
