#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <gmp.h>

#define OPTIONS "hvo:i:n:"

// Usage information of this program
// Return void
//
// *exec is the executale
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Encrypts data using SS encryption.\n"
        "   Encrypted data is decrypted by the decrypt program.\n"
        "\n"
        "USAGE\n"
        "   %s [OPTIONS]\n"
        "\n"
        "OPTIONS\n"
        "   -h              Display program help and usage.\n"
        "   -v              Display verbose program output.\n"
        "   -i infile       Input file of data to encrypt (default: stdin).\n"
        "   -o outfile      Output file for encrypted data (default: stdout).\n"
        "   -n pbfile       Public key file (default: ss.pub).\n",
        exec);
}

// Read in the public key and input messages, encrypt the messages and output
// as a file based on command-line options
// Return 0
//
// argc is the number of arguments
// **argv is the arguments
int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false; // No verbose output if not specified
    bool inp = false; // Input file not specified
    bool out = false; // Output file not specified
    char *pb = "ss.pub"; // Public key file name
    FILE *pbptr; // Public key file pointer
    char *input; // Input file name
    char *output; // Output file name
    FILE *finptr; // Input file pointer
    FILE *foutptr; // Output file pointer
    char user[256]; // storing username

    // Initialize mpz_t integer
    mpz_t n;
    mpz_init(n);

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // Parse the arguments
        switch (opt) {
        case 'n': pb = optarg; break; // Get public key file name
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

    // Open the public key file
    if ((pbptr = fopen(pb, "r")) == NULL) { // fopen returns null if the file doesn't exist
        perror("Error opening public key file.");
        exit(1);
    } else {
        ss_read_pub(n, user, pbptr); // Read in public key and username
    }

    // Open input file
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

    // Print details if "-v" is specified
    if (verbose == true) {
        printf("user = %s\n", user);
        gmp_printf("n  (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }

    // Encrypt message and output to file or STDOUT according to command-line options
    if (inp == false && out == false) {
        ss_encrypt_file(stdin, stdout, n);
    } else if (inp == false && out == true) {
        ss_encrypt_file(stdin, foutptr, n);
    } else if (inp == true && out == true) {
        ss_encrypt_file(finptr, foutptr, n);
    } else if (inp == true && out == false) {
        ss_encrypt_file(finptr, stdout, n);
    }

    // Close opened files
    fclose(pbptr);
    fclose(finptr);
    fclose(foutptr);

    // Clean mpz_t integer
    mpz_clear(n);

    return 0;
}
