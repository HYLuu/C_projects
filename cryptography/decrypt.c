#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "hvo:i:n:"

// Usage information of this program
// Return void
//
// *exec is the executale
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Decrypts data using SS decryption.\n"
        "   Encrypted data is encrypted by the encrypt program.\n"
        "\n"
        "USAGE\n"
        "   %s [OPTIONS]\n"
        "\n"
        "OPTIONS\n"
        "   -h              Display program help and usage.\n"
        "   -v              Display verbose program output.\n"
        "   -i infile       Input file of data to decrypt (default: stdin)\n"
        "   -o outfile      Output file for decrypted data (default: stdout).\n"
        "   -n pvfile       Private key file (default: ss.priv).\n",
        exec);
}

// Read in the encrypted messages and private key, decrypt the messages and output
// as STDOUT or file according to command-line options
// Return 0
//
// argc is the number of arguments
// **argv is the arguments
int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false; // No verbose output if not specified
    bool inp = false; // Input file not specified
    bool out = false; // Output file not specified
    char *pv = "ss.priv"; // Private key file name
    FILE *pvptr; // Private key file pointer
    char *input; // Input file name
    char *output; // Output file name
    FILE *finptr; // Input file pointer
    FILE *foutptr; // Output file pointer

    // Initialize mpz_t integers
    mpz_t d, pq;
    mpz_inits(d, pq, NULL);

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // Parse the arguments
        switch (opt) {
        case 'n': pv = optarg; break;
        case 'i':
            inp = true; // Input file specified
            input = optarg;
            break;
        case 'o':
            out = true; // Output file specified
            output = optarg;
            break;
        case 'v': verbose = true; break;
        case 'h': usage(argv[0]); exit(1); // Help information
        default: usage(argv[0]); return EXIT_FAILURE; // Default setting if get an unknown argument
        }
    }

    if ((pvptr = fopen(pv, "r")) == NULL) { // fopen returns null if the file doesn't exist
        perror("Error opening private key file.");
        exit(1);
    } else { // Read in private key from file if the file exists
        ss_read_priv(pq, d, pvptr);
    }

    // Open input file for reading
    if (inp == true) {
        if ((finptr = fopen(input, "r")) == NULL) {
            perror("Error opening input file.");
            exit(1);
        }
    } else {
        finptr = NULL;
    }

    // Open output file for writing
    foutptr = fopen(output, "w");

    // Print out details if "-v" is specified
    if (verbose == true) {
        gmp_printf("pq  (%zu bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d  (%zu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    // Read in messages, decrypt messages and output to STDOUT or a file
    // according to command-line options
    if (inp == false && out == false) {
        ss_decrypt_file(stdin, stdout, d, pq);
    } else if (inp == false && out == true) {
        ss_decrypt_file(stdin, foutptr, d, pq);
    } else if (inp == true && out == true) {
        ss_decrypt_file(finptr, foutptr, d, pq);
    } else if (inp == true && out == false) {
        ss_decrypt_file(finptr, stdout, d, pq);
    }

    // Close opened files
    fclose(pvptr);
    fclose(finptr);
    fclose(foutptr);

    // Clear mpz_t integers
    mpz_clears(d, pq, NULL);

    return 0;
}
