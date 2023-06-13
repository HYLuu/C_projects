#include "word.h"
#include "io.h"
#include "endian.h"
#include "code.h"
#include "buffer.h"

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

//
// Read up to to_read bytes from infile and store them in buf. Return the number of bytes actually
// read.
//
// Since read() may not read in as many bytes as you asked for, this function should continuously
// call read() and attempt to read as many bytes as it has not yet read. For instance, if to_read is
// 100 and the first read() call only reads 20 bytes, it should attempt to read 80 bytes the next
// time it calls read().
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int r = 0;
    int tmp;
    while (r < to_read) { // Read until read as many bytes as asked for
        tmp = read(infile, &buf[r], to_read - r);
        if (tmp <= 0) { // Stop if error or EOF
            break;
        }
        r += tmp;
    }
    return r;
}

//
// Write up to to_write bytes from buf into outfile. Return the number of bytes actually written.
//
// Similarly to read_bytes, this function will need to call write() in a loop to ensure that it
// writes as many bytes as possible.
//
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int w = 0;
    int tmp;
    while (w < to_write) { // Write until write as many bytes as asked for
        tmp = write(outfile, &buf[w], to_write - w);
        if (tmp <= 0) { // Stop if error or EOF
            break;
        }
        w += tmp;
    }
    return w;
}

//
// Read a file header from infile into *header.
//
// This function need not create any additional buffer to store the contents of the file header.
// File headers, like any struct or any value in C, are just represented by bytes in memory which
// means you can use read_bytes to read however many bytes the header consumes (use sizeof!) into
// *header.
//
// Since we decided that the canonical byte order for our headers is little-endian, this function
// will need to swap the byte order of both the header fields if it is run on a big-endian system.
// For example, here is how the 4 bytes of the magic number will look when written to the file:
//
// +------+------+------+------+
// | 0xAC | 0xBA | 0xAD | 0xBA |
// +------+------+------+------+
//
// A big-endian computer would interpret those bytes as the 4-byte number 0xACBAADBA, which is
// not what we want, so you would have to change the order of those bytes in memory. A little-endian
// computer will interpret that as 0xBAADBAAC.
//
// This function should also make sure the magic number is correct. Since it has no return value you
// may call assert() to do that, or print out an error message and exit the program, or use some
// other way to report the error.
//
void read_header(int infile, FileHeader *header) {
    // Read header from input
    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
    // Verify magic number
    assert(header->magic == MAGIC);
    if (!little_endian()) { // Ensure endianness
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
}

//
// Write a file header from *header to outfile. Like above, this function should swap the byte order
// of the header's two fields if necessary.
//
void write_header(int outfile, FileHeader *header) {
    if (!little_endian()) { // Ensure endianness
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
    // Write out header
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
}

//
// Read one symbol from infile into *sym. Return true if a symbol was successfully read, false
// otherwise.
//
// Reading one symbol at a time is slow, so this function will need to maintain a global buffer
// (an array) of BLOCK bytes. Most calls will only need to read a symbol out of that buffer, and
// then update some counter so that the function knows what position in the buffer it is at. If
// there are no more bytes in the buffer for it to return, it will have to call read_bytes to refill
// the buffer with fresh data. If this call fails then you cannot read a symbol and should return
// false.
//
bool read_sym(int infile, uint8_t *sym) {
    if ((total_syms % BLOCK) == 0) { // If all bytes in the buffer are read
        read_blk = read_bytes(
            infile, char_buf, BLOCK); // Read in a new block of bytes to buffer from input
    }

    // Read to *sym
    *sym = char_buf[total_syms % BLOCK];
    // Increment the counter
    total_syms += 1;

    // No more bytes to read
    if (read_blk < BLOCK && (total_syms % BLOCK) == read_blk) {
        return false;
        // Read till EOF
    } else if (read_blk == 0) {
        return false;
    }
    return true;
}

//
// Get the integer read from LSB
//
// Take an unsigned integer as inpiut
// Return an integer
//
unsigned int from_lsb(unsigned int n) {
    int b = 0;
    if (n == 0) { // 0 read from LSB is 0
        return 0;
    }
    // Save one left shift
    b += n & 1;
    n >>= 1;
    while (n != 0) {
        b <<= 1;
        b += n & 1;
        n >>= 1;
    }
    return b;
}

//
// Get the bit length of an integer
//
// Take an unsigned integer as inpiut
// Return an integer
//
unsigned int bit_length(unsigned int x) {
    unsigned int ct = 0;
    while (x) {
        ct += 1;
        x >>= 1; // Shift until x == 0
    }
    return ct;
}

//
// Get a bit mask of mask_len of length and one_len of 1s
// e.g. mask_len = 5, one_len = 3, mask = 11100
//
// mask_len is an unsigned int and one_len is an integer
// Return a bit mask integer
//
unsigned int bit_mask(unsigned int mask_len, int one_len) {
    int mask = 0;
    for (int i = 0; i < one_len; i += 1) {
        mask |= (1 << i);
    }
    mask <<= (mask_len - one_len);
    return mask;
}

//
// Write a pair -- bitlen bits of code, followed by all 8 bits of sym -- to outfile.
//
// This function should also use a buffer. It writes into individual bits in the buffer, starting
// with the least significant bit of the first byte, until the most significant bit of the first
// byte, and then the least significant bit of the second byte, and so on. You will need bitwise
// arithmetic to manipulate individual bits within your buffer -- consult asgn3/set.c if you don't
// remember how to do this.
//
// The first bit of code to be written is the least significant bit, and the same holds for sym.
//
// This function will need to track which *bit* in the buffer will be written to next. If it ever
// reaches the end of the buffer it needs to write out the contents of the buffer to outfile; you
// may use flush_pairs to do this.
//
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {

    uint16_t code_rev = from_lsb(code); // Code read from LSB
    code_rev <<= (bitlen - bit_length(code_rev)); // Make sure code has bitlen
    uint8_t sym_rev = from_lsb(sym); // Sym read from LSB

    // Write the code
    int start_pos = ((total_bits % (BLOCK * 8)) % 8)
                    + 1; // The position of start writing bits (the first empty one)
    int bit_idx = bitlen; // Counter for code
    int buff_index = ((total_bits % (BLOCK * 8)) / 8); // Current working byte index
    uint16_t cd_bi = 0; // Temp code bits read in a byte
    while (bit_idx != 0) {
        unsigned int mask = bit_mask(bit_idx, 1); // Bit mask for the target bit
        unsigned int write_buff = code_rev & mask; // Get the target bit
        cd_bi |= write_buff; // Connect all bits in a byte
        start_pos += 1;
        bit_idx -= 1;
        total_bits += 1;
        if ((start_pos % 8) == 1
            && total_bits != 1) { // Filled one byte and should go to the next byte
            if ((total_bits % (BLOCK * 8)) == 0) { // Filled the buffer
                bin_buf[buff_index] |= cd_bi; // Write the byte to buffer
                flush_pairs(outfile); // Write out everything in the buffer
                buff_index = 0; // Reset index
            } else {
                bin_buf[buff_index] |= cd_bi;
                buff_index += 1;
            }
            cd_bi = 0;
        } else {
            if (bit_idx == 0) {
                bin_buf[buff_index] |= cd_bi;
                cd_bi = 0;
            }
        }
    }

    // Write the sym
    int sym_idx = 8;
    uint16_t sm_bi = 0; // Temp sym bits read in a byte
    while (sym_idx != 0) {
        unsigned int mask = bit_mask(sym_idx, 1); // Bit mask for the target bit
        unsigned int write_buff = sym_rev & mask; // Get the target bit
        sm_bi |= write_buff;
        start_pos += 1;
        sym_idx -= 1;
        total_bits += 1;
        if ((start_pos % 8) == 1 && total_bits != 1) {
            if ((total_bits % (BLOCK * 8)) == 0) { // Filled the buffer
                bin_buf[buff_index] |= sm_bi;
                flush_pairs(outfile); // Write out everything in the buffer
                buff_index = 0; // Reset index
            } else {
                bin_buf[buff_index] |= sm_bi;
                buff_index += 1;
            }
            sm_bi = 0;
        } else {
            if (sym_idx == 0) {
                bin_buf[buff_index] |= sm_bi;
                sm_bi = 0;
            }
        }
    }
}

//
// Write any pairs that are in write_pair's buffer but haven't been written yet to outfile.
//
// This function will need to be called at the end of encode since otherwise those pairs would never
// be written. You don't have to, but it would be easy to make this function also work when called
// by write_pair, since otherwise you would write essentially the same code in two places.
//
// If not all bits of the last byte in your buffer have been written to, make sure that the
// unwritten bits are set to zero. An easy way to do this is by zeroing the entire buffer after
// flushing it every time.
//
void flush_pairs(int outfile) {
    if (((total_bits % (BLOCK * 8)) % 8) == 0) { // If total wrote bits is the multiple of 8
        write_bytes(outfile, bin_buf, (total_bits % (BLOCK * 8) / 8));
        memset(bin_buf, 0, BLOCK); // Zeroing the entire buffer
    } else {
        write_bytes(outfile, bin_buf, (total_bits % (BLOCK * 8) / 8) + 1);
        memset(bin_buf, 0, BLOCK);
    }
}

//
// Read only one bit from buffer
//
// Need the index of buffer and the position of the target bit (1-8) as inputs
// Return the read target bit
//
uint16_t read_a_bit(int buff_index, int pos) { // pos: from MSB to LSB <-
    unsigned int mask = bit_mask(pos, 1); // Bit mask for target bit
    return (bin_buf[buff_index] & mask);
}

//
// Read bitlen bits of a code into *code, and then a full 8-bit symbol into *sym, from infile.
// Return true if the complete pair was read and false otherwise.
//
// Like write_pair, this function must read the least significant bit of each input byte first, and
// will store those bits into the LSB of *code and of *sym first.
//
// It may be useful to write a helper function that reads a single bit from a file using a buffer.
//
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    if ((total_bits % (BLOCK * 8)) == 0) { // Read in a new block of bytes to buffer
        read_blk = read_bytes(infile, bin_buf, BLOCK);
    }

    int start_pos = ((total_bits % (BLOCK * 8)) % 8)
                    + 1; // Current position to read of the current byte (1-8)
    int bit_idx = bitlen; // Bits left to process for code
    int buff_index = ((total_bits % (BLOCK * 8)) / 8); // Current working byte index
    uint16_t cd_bi = 0; // Temp code bits read in a byte
    uint16_t cd_by = 0; // Temp code bits read across bytes
    while (bit_idx != 0) {
        cd_bi |= read_a_bit(
            buff_index, 8 - (start_pos % 8) + 1); // Connect the read bit with previously read bits
        start_pos += 1;
        bit_idx -= 1;
        total_bits += 1;
        if ((start_pos % 8) == 1 && total_bits != 1) { // Read a byte and going to the next byte
            if ((total_bits % (BLOCK * 8)) == 0) { // Finish reading all bytes in the buffer
                read_bytes(
                    infile, bin_buf, BLOCK); // Read a new block of bytes from input to buffer
                buff_index = 0; // Reset counter for buffer index
            } else {
                buff_index += 1;
            }
            cd_by |= (cd_bi <<= bit_idx); // Save the read bits from this byte
            cd_bi = 0;
        }
    }
    *code = from_lsb(cd_by | cd_bi); // Get code read from MSB

    int sym_idx = 8; // Bits left to process for sym
    uint16_t sm_bi = 0; // Temp code bits read in a byte
    uint16_t sm_by = 0; // Temp code bits read across bytes
    while (sym_idx != 0) {
        sm_bi |= read_a_bit(
            buff_index, 8 - (start_pos % 8) + 1); // Connect the read bit with previously read bits
        start_pos += 1;
        sym_idx -= 1;
        total_bits += 1;
        if ((start_pos % 8) == 1 && total_bits != 1) { // Read a byte and going to the next byte
            if ((total_bits % (BLOCK * 8)) == 0) { // Finish reading all bytes in the buffer
                read_bytes(
                    infile, bin_buf, BLOCK); // Read a new block of bytes from input to buffer
                buff_index = 0; // Reset counter for buffer index
            } else {
                buff_index += 1;
            }
            sm_by |= (sm_bi <<= sym_idx); // Save the read bits from this byte
            sm_bi = 0;
        }
    }
    *sym = from_lsb(sm_by | sm_bi); // Get sym read from MSB

    // Return false if there's no more to read from buffer or input
    if (read_blk < BLOCK && ((total_bits % (BLOCK * 8)) / 8) == read_blk) {
        return false;
    } else if (read_blk == 0) {
        return false;
    }

    return true;
}

//
// Write every symbol from w into outfile.
//
// These symbols should also be buffered and the buffer flushed whenever necessary (note you will
// likely sometimes fill up your buffer in the middle of writing a word, so you cannot only check
// that the buffer is full at the end of this function).
//
void write_word(int outfile, Word *w) {
    uint32_t sm_idx = w->len;
    while (sm_idx != 0) {
        memcpy(&char_buf[total_syms % BLOCK], &w->syms[w->len - sm_idx],
            1); // Write the target symbol to buffer
        sm_idx -= 1;
        total_syms += 1;
        if ((total_syms % BLOCK) == 0 && total_syms != 0) { // If buffer is fully written
            flush_words(outfile); // Write out all the bytes in buffer to output
        }
    }
}

//
// Write any unwritten word symbols from the buffer used by write_word to outfile.
//
// Similarly to flush_pairs, this function must be called at the end of decode since otherwise you
// would have symbols remaining in the buffer that were never written.
//
void flush_words(int outfile) {
    if (((total_syms % (BLOCK)) % 8) == 0) { // If total written word is the multiple of 8
        write_bytes(outfile, char_buf, ((total_syms % BLOCK) / 8)); // Write out the whole buffer
        memset(char_buf, 0, BLOCK); // Zero out the entire buffer
    } else {
        write_bytes(outfile, char_buf, ((total_syms % BLOCK) / 8) + 1);
        memset(char_buf, 0, BLOCK);
    }
}
