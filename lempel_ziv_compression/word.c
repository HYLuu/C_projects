#include "word.h"
#include "code.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//
// Creates a new Word with symbols syms and length len
// Allocates new array and copies the symbols over
//
Word *word_create(uint8_t *syms, uint32_t len) {
    Word *w;
    w = (Word *) calloc(1, sizeof(Word)); // Allocate memory for a new Word

    memcpy(w->syms, syms, len);
    w->len = len;

    if (!w) {
        return NULL;
    }
    return w;
}

//
// Creates a new word by appending symbol sym to word w
// Updates the length of the new word and copies symbols over
// Returns a pointer to the newly allocated word
//
Word *word_append_sym(Word *w, uint8_t sym) {
    uint8_t *new;
    Word *w_append;

    if (!w) { // If the word is empty
        new = (uint8_t *) calloc(1, sizeof(uint8_t));
        new[0] = sym; // One symbol only
        w_append = word_create(new, 1);
    } else { // If the word exists
        new = (uint8_t *) calloc((w->len + 1), sizeof(uint8_t));
        memcpy(new, w->syms, w->len); // Copy the original word
        new[w->len] = sym; // Append the new symbol
        w_append = word_create(new, w->len + 1);
    }

    return w_append;
}

//
// Deletes the word w
// Frees up associated space
//
void word_delete(Word *w) {
    free(w);
}

//
// Constructor:
// Creates a new table big enough to fit MAX_CODE
// Creates the first element at EMPTY_CODE and returns it
//
WordTable *wt_create(void) {
    WordTable *wt;
    wt = (WordTable *) calloc(MAX_CODE, sizeof(Word)); // Allocate memomy for a new WordTable

    uint8_t syms[] = { 0 }; // Empty sym
    wt[EMPTY_CODE] = word_create(syms, 0);
    return wt;
}

//
// Deletes all words except EMPTY_CODE
// Frees associated memory
//
void wt_reset(WordTable *wt) {
    for (int i = 0; i < MAX_CODE; i += 1) {
        word_delete(wt[i]); // Delete everything in WordTable
    }

    uint8_t syms[] = { 0 };
    wt[EMPTY_CODE] = word_create(syms, 0); // Reset EMPTY_CODE
}

//
// Destructor: Deletes all words and tables
// Frees up associated memory
//
void wt_delete(WordTable *wt) {
    free(wt);
}
