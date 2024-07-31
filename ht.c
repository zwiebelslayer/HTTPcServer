// Hashtable for the routes

#include "ht.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 64   // how big should the init hashtable be?

// SEE https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1089511628211UL

// Hash table entry
typedef struct {
    const char *key;
    void *value;
} ht_entry;

struct ht {
    ht_entry *entries;  // holds the entries array
    size_t capacity;    // size of entries array
    size_t length;     // current number of elements capacity >= length
};


// create the hashtable
struct ht *ht_create(void) {
    // alloc memory for the hashtable pointer
    struct ht *hash_table = (struct ht *) malloc(sizeof(struct ht));
    if (hash_table == NULL) {
        return NULL; // ERROR
    }
    hash_table->length = 0;
    hash_table->capacity = INITIAL_CAPACITY;
    // alloc memeory for the entries array
    ht_entry *array_for_entries = calloc(hash_table->capacity,sizeof(ht_entry));
    if (array_for_entries == NULL) {
        free(hash_table);
        return NULL; // ERROR
    }
    hash_table->entries = array_for_entries;
    return hash_table;
}

/*
 * Free the memory of the hashtable
 */

void ht_delete(struct ht *hash_table) {
    // free alloc keys
    for(size_t i=0; i< hash_table->capacity; i++){
        free((char*) hash_table->entries[i].key);
    }
    free(hash_table->entries);
    free(hash_table);
}


// hash the key using https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
uint64_t hash_key(const char* key){
    uint64_t hash_val = FNV_OFFSET;
    int size_of_key = strlen(key); // TODO strlen() is not needed just check for the NULL char at the end!
    for(int i=0; i<size_of_key; i++ ){ // TODO: could be solved better
        hash_val += hash_val * FNV_PRIME;
        hash_val = hash_val ^ key[i];
    }
    return hash_val;
}
