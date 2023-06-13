#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#define OPTIONS "hvb:i:n:d:s:"

// Usage information of this program
// Return void
//
// *exec is the executale
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Generates an SS public/private key pair.\n"
        "\n"
        "USAGE\n"
        "   %s [OPTIONS]\n"
        "\n"
        "OPTIONS\n"
        "   -h              Display program help and usage.\n"
        "   -v              Display verbose program output.\n"
        "   -b bits         Minimum bits needed for public key n (default: 256).\n"
        "   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n"
        "   -n pbfile       Public key file (default: ss.pub).\n"
        "   -d pvfile       Private key file (default: ss.priv)\n"
        "   -s seed         Random seed for testing.\n",
        exec);
}

// Generate and output the public key and private key basde on command-line options
// Return 0
//
// argc is the number of arguments
// **argv is the arguments
int main(int argc, char **argv) {
    int opt = 0;
    uint64_t bits = 256; // Minimum bits needed for the public modulus n
    uint64_t iters = 50; // Number of Miller-Rabin iterations for testing primes
    bool s = false; // "-s" not specified
    uint64_t seed = 0; // Random seed for the random state initialization
    bool verbose = false; // No verbose output if not specified
    char *pb = "ss.pub"; // Public key file name
    char *pv = "ss.priv"; // Private key file name
    FILE *pbptr; // Public key file pointer
    FILE *pvptr; // Private key file pointer

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // Parse the arguments
        switch (opt) {
        case 'b': bits = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'i': iters = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 's':
            s = true; // user apecified seed
            seed = (uint64_t) strtoul(optarg, NULL, 10);
            break;
        case 'v': verbose = true; break;
        case 'n': pb = optarg; break;
        case 'd': pv = optarg; break;
        case 'h': usage(argv[0]); exit(1); // Help information
        default: usage(argv[0]); return EXIT_FAILURE; // Default setting if get an unknown argument
        }
    }

    // File sfor writing public key and private keys
    pbptr = fopen(pb, "w");
    pvptr = fopen(pv, "w");
    if (pbptr == NULL) { // fopen returns null if the file doesn't exist
        perror("Error opening public key file.");
        exit(1);
    } else if (pvptr == NULL) {
        perror("Error opening private key file.");
        exit(1);
    }

    // Set the private key file permission to read and write for the user only
    fchmod(fileno(pvptr), S_IRUSR | S_IWUSR);

    // Set seed
    if (s == false) { // Default seed
        randstate_init(time(NULL));
    } else { // User specified seed
        randstate_init(seed);
    }

    // Initialize mpz_t integers
    mpz_t p, q, n, d, pq;
    mpz_inits(p, q, n, d, pq, NULL);
    // Generate public key and private key
    ss_make_pub(p, q, n, bits, iters);
    ss_make_priv(d, pq, p, q);

    // Get username
    char *user = getenv("USER");

    // Write out to files
    ss_write_pub(n, user, pbptr);
    ss_write_priv(pq, d, pvptr);

    // Print out verbose details if "-v" is specified
    if (verbose == true) {
        printf("user = %s\n", user);
        gmp_printf("p  (%zu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q  (%zu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n  (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("pq  (%zu bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d  (%zu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    // Close the opened files
    fclose(pbptr);
    fclose(pvptr);

    // Clear the mpz_t integers
    randstate_clear();
    mpz_clears(p, q, n, d, pq, NULL);

    return 0;
}
