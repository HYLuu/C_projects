#include "trie.h"
#include "code.h"

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

//
// Creates a new TrieNode and returns a pointer to it
// Allocate memory for TrieNode
// Code is the code to be assigned to this new node
// Returns the newly allocated node
//
TrieNode *trie_node_create(uint16_t index) {
    TrieNode *n;
    n = (TrieNode *) calloc(1, sizeof(TrieNode)); // Allocate memory for a trie node
    assert(n != NULL);

    TrieNode *child[ALPHABET]; // An array of Trie nodes
    for (int i = 0; i < ALPHABET; i += 1) {
        child[i] = NULL; // Set all Trie nodes to NULL
    }

    memcpy(n->children, child, sizeof(child));
    n->code = index;

    return n;
}

//
// Deletes Node n
// Frees any allocated memory
//
void trie_node_delete(TrieNode *n) {
    free(n);
}

//
// Constructor: Creates the root TrieNode and returns a pointer to it
// Allocate memory for TrieNode
// Code is START_CODE
// Returns the newly allocated node
//
TrieNode *trie_create(void) {
    TrieNode *n = trie_node_create(EMPTY_CODE);
    if (!n) {
        return NULL;
    };

    return n;
}

//
// Resets the trie: called when code reaches MAX_CODE
// Deletes all the children of root and frees allocated memory
//
void trie_reset(TrieNode *root) {
    for (int i = 0; i < ALPHABET; i += 1) {
        if (root->children[i]) { // Delete all existing nodes and their children
            trie_node_delete(root->children[i]);
        }
    }
}

//
// Destructor: Deletes all nodes starting at n as the root
// Deletes any children recursively before deleting the root
// Frees all the memory allocated for TrieNodes n and below
//
void trie_delete(TrieNode *n) {
    if (n == NULL) { // The node doesn't exist
        return;
    } else if (n != NULL) {
        for (int i = 0; i < ALPHABET; i += 1) {
            if (n->children[i]) { // Delete all existing nodes and their children
                trie_delete(n->children[i]);
            } else { // Delete the leaf
                trie_node_delete(n->children[i]);
            }
        }
    }
}

//
// Checks if node has any children called sym
// Returns the address if found, NULL if absent
//
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    if (n->children[sym] == NULL) {
        return NULL;
    }
    return n->children[sym];
}
