#include "code.h"
#include "endian.h"
#include "trie.h"
#include "word.h"
#include "io.h"
#include "buffer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#define OPTIONS "hvo:i:"

uint64_t total_syms = 0;
uint64_t total_bits = 0;
int read_blk = 0;
uint8_t *bin_buf;
uint8_t *char_buf;

unsigned int bit_length(unsigned int x);

// Usage information of this program
// Return void
//
// *exec is the executale
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Compresses files using the LZ78 compression algorithm.\n"
        "   Compressed files are decompressed with the corresponding decoder.\n"
        "\n"
        "USAGE\n"
        "   %s [-vh] [-i input] [-o output]\n"
        "\n"
        "OPTIONS\n"
        "   -v          Display compression statistics\n"
        "   -i input    Specify input to compress (stdin by default)\n"
        "   -o output   Specify output of compressed input (stdout by default)\n"
        "   -h          Display program help and usage\n",
        exec);
}

// Raed in symbols in the input, compress the messages and write the compressed messages to output
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
    struct stat infile_stat; // Input file info

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
        } else { // Open successfully
            if (fstat(infile, &infile_stat) < 0) { // Get input information
                perror("infile stat error");
                exit(1);
            }
        }
    } else { // If getting input from STDIN
        infile = 0;
        if (fstat(infile, &infile_stat) < 0) {
            perror("infile stat error");
            exit(1);
        }
    }

    // Initialize header struct
    FileHeader *header;
    header = (FileHeader *) calloc(1, sizeof(FileHeader));
    header->magic = MAGIC;
    header->protection = infile_stat.st_mode & 07777;

    // Open output file if specified
    if (out == true) {
        outfile = open(output, O_WRONLY | O_CREAT, 0777);
        if (outfile < 0) { // Unable to open
            perror("Error opening output file:"); // Show error message
            exit(1);
        }
    } else { // If output to STDOUT
        outfile = 1;
    }

    // Write out header
    write_header(outfile, header);

    // Create a Trie
    TrieNode *main_trie = trie_node_create(EMPTY_CODE);
    TrieNode *curr_node = main_trie; // Make a copy

    uint16_t next_code = START_CODE;
    TrieNode *prev_node = NULL;
    TrieNode *next_node = NULL;
    uint8_t prev_sym = 0;
    uint8_t curr_sym = 0;

    // Allocate space for buffers
    if (!(bin_buf = (uint8_t *) calloc(BLOCK, sizeof(uint8_t)))) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    if (!(char_buf = (uint8_t *) calloc(BLOCK, sizeof(uint8_t)))) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    while (read_sym(infile, &curr_sym) == true) { // Read symbols from input until no more to read
        next_node = trie_step(curr_node, curr_sym); // Get the next node on Trie
        if (next_node != NULL) { // We have seen the current prefix before
            prev_node = curr_node;
            curr_node = next_node;
        } else { // Haven't seen the prefix before
            write_pair(outfile, curr_node->code, curr_sym,
                bit_length(next_code)); // Write out the current pair
            curr_node->children[curr_sym] = trie_node_create(next_code); // New Trie node
            curr_node = main_trie; // Reset current node
            next_code += 1;
        }

        if (next_code == MAX_CODE) {
            trie_reset(main_trie);
            curr_node = main_trie;
            next_code = START_CODE;
        }
        prev_sym = curr_sym;
    }

    if (curr_node != main_trie) { // If still matching a prefix
        write_pair(outfile, prev_node->code, prev_sym, bit_length(next_code));
        next_code
            = (next_code + 1) % MAX_CODE; // Ensur next code stays within the limit of MAX_CODE
    }

    // Signal the end of the output
    write_pair(outfile, STOP_CODE, 0, bit_length(next_code));
    flush_pairs(outfile);

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
