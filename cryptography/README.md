# Assignment 5 - Cryptography

## Program Description

The three main program will be the implementation of public key cryptography using Schmidt-Samoa (SS) algorithm. The *keygen.c* is a key generator that produces SS public and private key pairs. The *encrypt.c* program is an encryptor that encrypts files using a public key, while the *decrypt.c* program decrypts the encrypted files using the corresponding private key. <br /> 

The *randstate.c* contains the implementation of a random state module. The *numtheory.c* program contains the implementations of the number theory functions behind SS. The *ss.c* contains implementations of routines for SS. The above libraries and module will be used in the three main programs.

## Building encrypt, decrypt, and keygen executables

```
$ make all
```
will create executable binary files titled 'keygen', 'encrypt' and 'decrypt'. They will generate public and private keys, encrypt input messages and decrypt the encrypted message. This commands will as well compile and link the random state module and libraries.

## Building keygen

```
$ make keygen
```
will only create executable binary files titled 'keygen'.

## Building encrypt

 ```
 $ make encrypt
 ```
will only create executable binary files titled 'encrypt'.

## Building decrypt

 ```
$ make decrypt
 ```
will only create executable binary files titled 'decrypt'.

## Running keygen

```
$ ./keygen [-options]
```
This command without any argument will generate public and private keys using default settings. <br />

The options of *keygen*: <br />

-h              Display program help and usage. <br />
-v              Display verbose program output. <br />
-b bits         Minimum bits needed for public key n (default: 256). <br />
-i iterations   Miller-Rabin iterations for testing primes (default: 50). <br />
-n pbfile       Public key file (default: ss.pub). <br />
-d pvfile       Private key file (default: ss.priv) <br />
-s seed         Random seed for testing. <br />

## Running encrypt

 ```
$ ./encrypt [-options]
 ```
If no command-line argument is specified, the program will wait for user input data. <br />

The options of *encrypt*: <br />

-h              Display program help and usage. <br />
-v              Display verbose program output. <br />
-i infile       Input file of data to encrypt (default: stdin). <br />
-o outfile      Output file for encrypted data (default: stdout). <br />
-n pbfile       Public key file (default: ss.pub). <br />

## Running decrypt

 ```
$ ./decrypt [-options]
 ```
If no command-line argument is specified, the program will wait for user input data. <br />

The options of *decrypt*: <br />

-h              Display program help and usage. <br />
-v              Display verbose program output. <br />
-i infile       Input file of data to decrypt (default: stdin) <br />
-o outfile      Output file for decrypted data (default: stdout). <br />
-n pvfile       Private key file (default: ss.priv). <br />

## Cleaning

```
$ make clean
```
will clean the directory after building. It will remove the executable 'keygen', 'encrypt', 'decrypt' and the .o files from the directory.

## Formatting

```
$ make format
```
will format all the source code, including the header files.
