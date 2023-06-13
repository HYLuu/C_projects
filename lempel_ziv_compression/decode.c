#include "code.h"
#include "endian.h"
#include "trie.h"
#include "word.h"
#include "io.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define OPTIONS "hvo:i:"

uint64_t total_syms = 0;
uint64_t total_bits = 0;
int read_blk = 0;
uint8_t *bin_buf;
uint8_t *char_buf;

uint16_t bit_length(uint16_t x);

// Usage information of this program
// Return void
//
// *exec is the executale
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Decompresses files with the LZ78 decompression algorithm.\n"
        "   Used with files compressed with the corresponding encoder.\n"
        "\n"
        "USAGE\n"
        "   %s [-vh] [-i input] [-o output]\n"
        "\n"
        "OPTIONS\n"
        "   -v          Display decompression statistics\n"
        "   -i input    Specify input to decompress (stdin by default)\n"
        "   -o output   Specify output of decompressed input (stdout by default)\n"
        "   -h          Display program usage\n",
        exec);
}

// Read in compressed messages, convert them into original content and write to output
// based on command-line options
// Return 0
//
// argc is the number of arguments
// **argv is the arguments
int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false; // No verbose output if not specified
    bool inp = false; // Input file not specified
    bool out = false; // Output file not specified
    char *input; // Input file name
    char *output; // Output file name
    int infile; // Input file descriptor
    int outfile; // Output file descriptor

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // Parse the arguments
        switch (opt) {
        case 'i':
            inp = true;
            input = optarg; // Get input file name
            break;
        case 'o':
            out = true;
            output = optarg; // Get output file name
            break;
        case 'v': verbose = true; break;
        case 'h': usage(argv[0]); exit(1); // Help information
        default: usage(argv[0]); return EXIT_FAILURE; // Default setting if get an unknown argument
        }
    }

    // Open input file if specified
    if (inp == true) {
        infile = open(input, O_RDONLY);
        if (infile < 0) { // Unable to open
            perror("Error opening input file:"); // Show error message
            exit(1);
        }
    } else { // If getting input from STDIN
        infile = 0;
    }

    // Initialize header struct
    FileHeader *header;
    header = (FileHeader *) calloc(1, sizeof(FileHeader));
    read_header(infile, header); // Read in header

    // Open output file if specified
    if (out == true) {
        outfile = open(output, O_WRONLY | O_CREAT);
        if (outfile < 0) { // Unable to open
            perror("Error opening output file:"); // Show error message
            exit(1);
        }
    } else { // If output to STDOUT
        outfile = 1;
    }

    // Set permission of output
    fchmod(outfile, header->protection);

    // Create a WordTable
    WordTable *table = wt_create();

    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;
    uint8_t curr_sym = 0;
    total_syms = 0;
    total_bits = 0;

    // Allocate space for buffers
    if (!(bin_buf = (uint8_t *) calloc(BLOCK, sizeof(uint8_t)))) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    if (!(char_buf = (uint8_t *) calloc(BLOCK, sizeof(uint8_t)))) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    // Read pairs from input until no more to read
    while (read_pair(infile, &curr_code, &curr_sym, bit_length(next_code)) == true) {
        table[next_code] = word_append_sym(table[curr_code],
            curr_sym); // Append the read symbol with the word denoted by the read code
        write_word(outfile, table[next_code]); // Write out the word at the next code to output
        next_code += 1;
        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }

    // Write out remaining words in the buffer
    flush_words(outfile);

    if (verbose == true) {
        printf("Compressed file size: %" PRIu64 " bytes\n", (total_bits / 8));
        printf("Uncompressed file size: %" PRIu64 " bytes\n", total_syms);
        printf("Compression ratio: %4.2f %%\n",
            (100 * (1 - ((float) (total_bits / 8) / (float) total_syms))));
    }

    close(infile);
    close(outfile);
    free(bin_buf);
    free(char_buf);

    return 0;
}
