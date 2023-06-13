#include "mathlib.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

// Usage information of this program.
// Return void
//
// *exec is the executale
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   A test harness for the small numerical library.\n"
        "\n"
        "USAGE\n"
        "   %s [-aebmrvnsh]\n"
        "\n"
        "OPTIONS\n"
        "   -a         Runs all tests.\n"
        "   -e         Runs e test.\n"
        "   -b         Runs BBP pi test.\n"
        "   -m         Runs Madhava pi test.\n"
        "   -r         Runs Euler pi test.\n"
        "   -v         Runs Viete pi test.\n"
        "   -n         Runs Newton square root tests.\n"
        "   -s         Print verbose statistics.\n"
        "   -h         Display program synopsis and usage.\n",
        exec);
}

double pi = M_PI; // Constant π in math.h
double e_const = M_E; // Constant e in math.h

// Print results of e() and e_terms()
// Return void
//
// k is an integer to determine whether to print e_terms()
void get_e(int k) {
    if (k == 0) {
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e(), e_const,
            absolute(e() - e_const));
    } else {
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e(), e_const,
            absolute(e() - e_const));
        printf("e() terms = %d\n", e_terms());
    }
}

// Print results of pi_madhava() and pi_madhava_terms()
// Return void
//
// k is an integer to determine whether to print pi_madhava_terms()
void get_m(int k) {
    if (k == 0) {
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_madhava(), pi,
            absolute(pi_madhava() - pi));
    } else {
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_madhava(), pi,
            absolute(pi_madhava() - pi));
        printf("pi_madhava() terms = %d\n", pi_madhava_terms());
    }
}

// Print results of pi_euler() and pi_euler_terms()
// Return void
//
// k is an integer to determine whether to print pi_euler_terms()
void get_r(int k) {
    if (k == 0) {
        printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_euler(), pi,
            absolute(pi_euler() - pi));
    } else {
        printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_euler(), pi,
            absolute(pi_euler() - pi));
        printf("pi_euler() terms = %d\n", pi_euler_terms());
    }
}

// Print results of pi_bbp() and pi_bbp_terms()
// Return void
//
// k is an integer to determine whether to print pi_bbp_terms()
void get_b(int k) {
    if (k == 0) {
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_bbp(), pi,
            absolute(pi_bbp() - pi));
    } else {
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_bbp(), pi,
            absolute(pi_bbp() - pi));
        printf("pi_bbp() terms = %d\n", pi_bbp_terms());
    }
}

// Print results of pi_viete() and pi_viete_factors()
// Return void
//
// k is an integer to determine whether to print pi_viete_factors()
void get_v(int k) {
    if (k == 0) {
        printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_viete(), pi,
            absolute(pi_viete() - pi));
    } else {
        printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_viete(), pi,
            absolute(pi_viete() - pi));
        printf("pi_viete() terms = %d\n", pi_viete_factors());
    }
}

// Print results of sqrt_newton() and sqrt_newton_iters()
// Return void
//
// k is an integer to determine whether to print sqrt_newton_iters()
void get_n(int k) {
    if (k == 0) {
        for (double i = 0.0; i < 10.1; i = i + 0.1) { // from 0.0 to 10.0, each step is 0.1
            printf("sqrt_newton(%7.6lf) = %16.15lf, sqrt(%7.6lf) = %16.15lf, diff = %16.15lf\n", i,
                sqrt_newton(i), i, sqrt(i), absolute(sqrt_newton(i) - sqrt(i)));
        }
    } else {
        for (double i = 0.0; i < 10.1; i = i + 0.1) {
            printf("sqrt_newton(%7.6lf) = %16.15lf, sqrt(%7.6lf) = %16.15lf, diff = %16.15lf\n", i,
                sqrt_newton(i), i, sqrt(i), absolute(sqrt_newton(i) - sqrt(i)));
            printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
        }
    }
}

// Print results of math inplementations according to command-line options
// Return void
//
// argc is the number of arguments
// **argv is the arguments
int main(int argc, char **argv) {
    int opt = 0;
    int index;
    int present = 0; // Variable to check if receive "-s"

    // Print out usage information if there is no argument
    if (argc == 1) {
        usage(argv[0]);
    }

    // Walk through each argument to check if option "-s" exists
    for (index = optind; index < argc; index++) {
        if (strcmp(argv[index], "-s") == 0) {
            present = 1; // Variable present =1 when "s" exists
            break;
        }
    }

    // When "-s" exists, print out iteration statistics for every requested tests from command-line
    if (present == 1) {
        while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // Parse the arguments
            switch (opt) { // Specify the content of each option
            case 'e': get_e(1); break;
            case 'm': get_m(1); break;
            case 'r': get_r(1); break;
            case 'b': get_b(1); break;
            case 'v': get_v(1); break;
            case 'n': get_n(1); break;
            case 'a': // Print all
                get_e(1);
                get_r(1);
                get_b(1);
                get_m(1);
                get_v(1);
                get_n(1);
                break;
            case 's':
                if (argc == 2) { // Print the usage information if no other options is given
                    usage(argv[0]);
                    break;
                } else {
                    break;
                }
            case 'h': usage(argv[0]); break; // Help information
            default:
                usage(argv[0]);
                return EXIT_FAILURE; // Default setting if get an unknown argument
            }
        }
    } else { // No "-s" argument
        while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // Parse the arguments
            switch (opt) { // Specify the content of each option
            case 'e': get_e(0); break;
            case 'm': get_m(0); break;
            case 'r': get_r(0); break;
            case 'b': get_b(0); break;
            case 'v': get_v(0); break;
            case 'n': get_n(0); break;
            case 'a':
                get_e(0);
                get_r(0);
                get_b(0);
                get_m(0);
                get_v(0);
                get_n(0);
                break;
            case 's':
                if (argc == 2) { // Print the usage information if no other options is given
                    usage(argv[0]);
                    break;
                } else {
                    break;
                }
            case 'h': usage(argv[0]); break; // Help information
            default:
                usage(argv[0]);
                return EXIT_FAILURE; // Default setting if get an unknown argument
            }
        }
    }
}
