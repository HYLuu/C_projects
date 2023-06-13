#include "numtheory.h"
#include "randstate.h"

// Check if input is odd number
// Return boolean
//
// d is a mpz_t integer
bool is_odd(mpz_t d) {
    mpz_t x, o;
    mpz_init_set_ui(x, 1);
    mpz_init(o);
    mpz_and(o, d, x);
    if (mpz_cmp_ui(o, 1) == 0) {
        mpz_clears(o, x, NULL);
        return true;
    } else {
        mpz_clears(o, x, NULL);
        return false;
    }
}

// Check if input is even number
// Return boolean
//
// d is a mpz_t integer
bool is_even(mpz_t d) {
    mpz_t x, o;
    mpz_init_set_ui(x, 1);
    mpz_init(o);
    mpz_and(o, d, x);
    if (mpz_cmp_ui(o, 0) == 0) {
        mpz_clears(o, x, NULL);
        return true;
    } else {
        mpz_clears(o, x, NULL);
        return false;
    }
}

// Compute the greatest common divisor gcd(a, b)
// Return void
//
// g is the greatest common divisor, should be initialized first
// a and b are mpz_t integers
void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t, a_, b_;
    mpz_inits(t, a_, b_, NULL);
    mpz_set(a_, a);
    mpz_set(b_, b);
    while (mpz_cmp_ui(b_, 0) != 0) {
        mpz_set(t, a_); // t = a
        mpz_set(a_, b_); // a = b
        mpz_mod(b_, t, b_); // b = t % b
    }
    mpz_set(g, a_);
    mpz_clears(t, a_, b_, NULL);
}

// Compute the muliplicative inverse of a (mod n)
// Return void
//
// o is the muliplicative inverse of a (mod n), should be initialized first
// a and n are mpz_t integers
void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r, rP, t, tP, q, tmp, qrP, qtP;
    mpz_inits(t, q, tmp, qrP, qtP, NULL);
    mpz_init_set(r, n);
    mpz_init_set(rP, a);
    mpz_init_set_ui(tP, 1);
    while (mpz_cmp_ui(rP, 0) != 0) {
        mpz_fdiv_q(q, r, rP);
        mpz_set(tmp, rP);
        mpz_mul(qrP, q, rP);
        mpz_sub(rP, r, qrP);
        mpz_set(r, tmp);
        mpz_set(tmp, tP);
        mpz_mul(qtP, q, tP);
        mpz_sub(tP, t, qtP);
        mpz_set(t, tmp);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0);
    } else {
        if (mpz_cmp_ui(t, 0) < 0) {
            mpz_add(o, t, n);
        } else {
            mpz_set(o, t);
        }
    }
    mpz_clears(r, rP, t, tP, q, tmp, qrP, qtP, NULL);
}

// Calculate a^b (mod n)
// Return void
//
// o is the calculation result, should be initialized first
// a, d, n are mpz_t integers
void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t v, p, vp, pp, d_;
    mpz_init_set_ui(v, 1);
    mpz_init_set(p, a);
    mpz_init_set(d_, d);
    mpz_inits(vp, pp, NULL);
    while (mpz_cmp_ui(d_, 0) > 0) {
        if (is_odd(d_)) {
            mpz_mul(vp, v, p);
            mpz_mod(v, vp, n);
        }
        mpz_mul(pp, p, p);
        mpz_mod(p, pp, n);
        mpz_fdiv_q_ui(d_, d_, 2);
    }
    mpz_set(o, v);
    mpz_clears(v, p, vp, pp, NULL);
}

// Factors n into the form d * 2 ** r
// Return void
//
// r, s, n are mpz_t integers
void get_r_s(mpz_t r, mpz_t s, mpz_t n) {
    mpz_set(r, n);
    mpz_set_ui(s, 0);
    while (mpz_even_p(r)) {
        mpz_fdiv_q_ui(r, r, 2);
        mpz_add_ui(s, s, 1);
    }
}

// Check if an integer is a prime using Miller-Rabin
// Return boolean
//
// n is a mpz_t integer
// iters is the iteration number
bool is_prime(const mpz_t n, uint64_t iters) {
    mpz_t a, r, s, n_, y, nt, n1, j, k, s_;
    mpz_inits(a, r, s, n_, y, nt, n1, j, k, s_, NULL);
    mpz_set(n_, n); // n
    mpz_set_ui(k, 2); // k = 2
    mpz_sub_ui(nt, n_, 3); // n - 3
    mpz_sub_ui(n1, n_, 1); // n - 1
    get_r_s(r, s, n1); // Factor n - 1 into r * 2**s
    mpz_sub_ui(s_, s, 1); // s - 1

    if (mpz_cmp_ui(n, 2) < 0 || (mpz_cmp_ui(n, 2) != 0 && mpz_even_p(n_))) {
        return false; // 0 & 1 & even numbers are not prime
    }
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        return true; // 2 & 3 are prime
    }
    for (uint64_t i = 0; i < iters; i += 1) {
        mpz_urandomm(a, state, nt); //random number in range [0, n - 4]
        mpz_add_ui(a, a, 2); // move range to [2, n - 2]
        pow_mod(y, a, r, n_);
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, n1) != 0) {
            mpz_set_ui(j, 1);
            while (mpz_cmp(j, s_) <= 0 && mpz_cmp(y, n1) != 0) {
                pow_mod(y, y, k, n_);
                if (mpz_cmp_ui(y, 1) == 0) {
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, n1) != 0) {
                return false;
            }
        }
    }
    mpz_clears(a, r, s, n_, y, nt, n1, j, k, s_, NULL);
    return true;
}

// Generate a prime
// Return void
//
// o is the result prime, should be initialized first
// The generated prime shoulb be at least have bits bits
// iters is the iteration number of generating random numbers
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t r, g, bt, tp;
    mpz_inits(r, g, bt, NULL);
    mpz_init_set_ui(tp, 1);
    do {
        mpz_urandomb(g, state, bits + 2); // Get a random integer
        mpz_mul_2exp(bt, tp, bits - 1);
        mpz_ior(r, g, bt);
    } while (!is_prime(r, iters)); // Check if it's prime
    mpz_set(p, r);
    mpz_clears(r, g, bt, tp, NULL);
}
