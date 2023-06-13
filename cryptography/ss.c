#include "ss.h"
#include "numtheory.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
//
// Generates the components for a new SS key.
//
// Provides:
//  p:  first prime
//  q: second prime
//  n: public modulus/exponent
//
// Requires:
//  nbits: minimum # of bits in n
//  iters: iterations of Miller-Rabin to use for primality check
//  all mpz_t arguments to be initialized
//
void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    // Initialization
    mpz_t p_, q_, mp, mq;
    mpz_inits(p_, q_, mp, mq, NULL);
    uint64_t low = nbits / 5;
    uint64_t high = (2 * nbits) / 5;
    // Get a random number in the range [nbits/5, (2 × nbits)/5) for bits of p
    uint64_t p_bit = random() % (high - low) + low;
    uint64_t q_bit = nbits - (2 * p_bit); // n = p * p * q
    make_prime(p, p_bit, iters); // Get p
    make_prime(q, q_bit, iters); // Get q
    mpz_sub_ui(p_, p, 1); // p - 1
    mpz_sub_ui(q_, q, 1); // q - 1
    mpz_mod(mp, q_, p);
    mpz_mod(mq, p_, q);
    // Check p !| (q − 1) and q !| (p − 1)
    while (mpz_cmp(p, q) == 0 || mpz_cmp_ui(mp, 0) == 0 || mpz_cmp_ui(mq, 0) == 0) {
        make_prime(q, q_bit, iters);
    }
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);
    mpz_clears(p_, q_, mp, mq, NULL);
}

//
// Generates components for a new SS private key.
//
// Provides:
//  d:  private exponent
//  pq: private modulus
//
// Requires:
//  p:  first prime number
//  q: second prime number
//  all mpz_t arguments to be initialized
//
void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t p_, q_, g, ab, n, lcm;
    mpz_inits(p_, q_, g, ab, n, lcm, NULL);
    mpz_sub_ui(p_, p, 1); // p - 1
    mpz_sub_ui(q_, q, 1); // q - 1
    // Get lcm(p - 1, q - 1)
    mpz_mul(ab, p_, q_);
    gcd(g, p_, q_);
    mpz_fdiv_q(lcm, ab, g);
    mpz_mul(pq, p, q); // pq = p * q
    // n = p * p * q
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);
    // inverse of n modulo λ(pq) = lcm(p − 1, q − 1)
    mod_inverse(d, n, lcm);
    mpz_clears(p_, q_, g, ab, n, lcm, NULL);
}

//
// Export SS public key to output stream
//
// Requires:
//  n: public modulus/exponent
//  username: login name of keyholder ($USER)
//  pbfile: open and writable file stream
//
void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n); // Print n
    fprintf(pbfile, "%s\n", username); // Print username
}

//
// Export SS private key to output stream
//
// Requires:
//  pq: private modulus
//  d:  private exponent
//  pvfile: open and writable file stream
//
void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", pq); // Print pq
    gmp_fprintf(pvfile, "%Zx\n", d); // Print d
}

//
// Import SS public key from input stream
//
// Provides:
//  n: public modulus
//  username: $USER of the pubkey creator
//
// Requires:
//  pbfile: open and readable file stream
//  username: requires sufficient space
//  all mpz_t arguments to be initialized
//
void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx", n); // Read in n
    fscanf(pbfile, "%s", username); // Read in username
}

//
// Import SS private key from input stream
//
// Provides:
//  pq: private modulus
//  d:  private exponent
//
// Requires:
//  pvfile: open and readable file stream
//  all mpz_t arguments to be initialized
//
void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx", pq); // Read in pq
    gmp_fscanf(pvfile, "%Zx", d); // Read in d
}

//
// Encrypt number m into number c
//
// Provides:
//  c: encrypted integer
//
// Requires:
//  m: original integer
//  n: public exponent/modulus
//  all mpz_t arguments to be initialized
//
void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

//
// Encrypt an arbitrary file
//
// Provides:
//  fills outfile with the encrypted contents of infile
//
// Requires:
//  infile: open and readable file stream
//  outfile: open and writable file stream
//  n: public exponent and modulus
//
void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    // Initialize mpz integers
    mpz_t k, m, c, base, exp, n_;
    mpz_inits(k, m, c, base, exp, n_, NULL);
    mpz_set_ui(base, 2); // base = 2
    mpz_set(n_, n); // n_ = n
    // Get log_2(n)
    while (mpz_cmp(n_, base) > 0 || mpz_cmp(n_, base) == 0) {
        mpz_add_ui(exp, exp, 1);
        mpz_tdiv_q(n_, n_, base);
    }
    // k = ((log2(sqrt(n)) - 1) / 8)
    mpz_fdiv_q_ui(k, exp, 2);
    mpz_sub_ui(k, k, 1);
    mpz_fdiv_q_ui(k, k, 8);
    uint64_t k_ = mpz_get_ui(k); // mpz_t -> uint64_t
    uint8_t *block = (uint8_t *) calloc(k_, sizeof(uint8_t)); // Allocate memory for read in block
    block[0] = 0xff; // First byte
    size_t j = 0;
    do {
        j = fread(&block[1], sizeof(uint8_t), k_ - 1,
            infile); // Read in (k_ - 1) length of bytes to block
        if (j != 0) {
            mpz_import(m, j + 1, 1, sizeof(block[0]), 1, 0, block); // Convert block to mpz_t m
            ss_encrypt(c, m, n); // m encrypted to c
            gmp_fprintf(outfile, "%Zx\n", c);
        }
    } while (j != 0);
    // Free
    free(block);
}

//
// Decrypt number c into number m
//
// Provides:
//  m: decrypted/original integer
//
// Requires:
//  c: encrypted integer
//  d: private exponent
//  pq: private modulus
//  all mpz_t arguments to be initialized
//
void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

//
// Decrypt a file back into its original form.
//
// Provides:
//  fills outfile with the unencrypted data from infile
//
// Requires:
//  infile: open and readable file stream to encrypted data
//  outfile: open and writable file stream
//  d: private exponent
//  pq: private modulus
//
void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    mpz_t k, m, c, base, exp, n_;
    mpz_inits(k, m, c, base, exp, n_, NULL);
    mpz_set_ui(base, 2);
    mpz_set(n_, pq);
    // Get log_2(pq)
    while (mpz_cmp(n_, base) > 0 || mpz_cmp(n_, base) == 0) {
        mpz_add_ui(exp, exp, 1);
        mpz_tdiv_q(n_, n_, base);
    }
    // k = ((log2(pq) - 1) / 8)
    mpz_sub_ui(k, exp, 1);
    mpz_fdiv_q_ui(k, k, 8);
    uint64_t k_ = mpz_get_ui(k); // mpz_t -> uint64_t
    uint8_t *block
        = (uint8_t *) calloc(k_ + 1, sizeof(uint8_t)); // Allocate memory for read in hexstring
    while (true) {
        int input = gmp_fscanf(infile, "%Zx", c);
        if (input == 1) {
            ss_decrypt(m, c, d, pq); // c decrypted to m
            size_t tmp = (mpz_sizeinbase(m, 2) / 8); // Read in size
            size_t *j = &tmp;
            mpz_export(
                block, j, 1, sizeof(block[0]), 1, 0, m); // Convert m to bytes and store in block
            for (uint64_t i = 1; i < *j; i += 1) {
                fprintf(outfile, "%c", block[i]);
            }
        } else if (input == EOF) {
            break;
        }
    }
    free(block);
}
