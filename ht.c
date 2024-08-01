// Hashtable for the routes

#include "ht.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 64   // how big should the init hashtable be MUST BE POWER OF 2

// SEE https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1089511628211UL



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

/*
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
*/


static uint64_t hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

/*
 * Get an entry from the hash table
 */
void* ht_get(struct ht* table, const char* key){
    uint64_t hashed_key = hash_key(key);
    size_t index = (size_t)(hashed_key & (uint64_t)(table->capacity - 1)); // extract the "lower" bit from the hash

    // Loop till we find an empty entry.
    while (table->entries[index].key != NULL) {
        if (strcmp(key, table->entries[index].key) == 0) {
            // Found key, return value.
            return table->entries[index].value;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= table->capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }
    return NULL;
}

/*
 * Insert an entry into the hash table
 */
const char*  ht_set(struct  ht* table, const char* key, void* value){
    if(value == NULL){
        return NULL; // ERROR, when inserting!
    }

    // If length will exceed half of current capacity, expand it.
    if (table->length >= table->capacity / 2) {
        if (!ht_expand(table)) {
            return NULL;
        }
    }

    // Set entry and update length.
    return ht_set_entry(table->entries, table->capacity, key, value,
                        &table->length);
}

static const char* ht_set_entry(ht_entry* entries, size_t capacity, const char* key, void* value, size_t* ht_size){
    uint64_t hashed_key = hash_key(key);
    size_t index = hashed_key & ((uint64_t )(capacity -1 )); // extract the "lower" bit from the hash


    // Loop through hashtable to search empty spot, beginning with the calc index (that would be ideal)
    // this function is called from ht_set -> there always MUST be atleast one EMPTY bucket!
    while(entries[index].key != NULL){

        if (strcmp(key, entries[index].key) == 0) {
            // update existing value, since the key is the same
            entries[index].value = value;
            return entries[index].key;
        }

        index++; // roll over to next bucket

        if(index >= capacity){
            // rollover to zero
            index = 0;
        }
    }

    // Update hash table size, only for the insert function but now for the ht_expand function
    if( ht_size != NULL){
        key = strdup(key); // copy key
        if(key == NULL){
            return NULL;
        }
        (*ht_size) ++; // increase ht_size!
    }

    // found empty spot insert value
    entries[index].key = (char*) key; // store key
    entries[index].value = value;

    return key; // return key, when successful

}

/*
 * Expands the hashtable by the next Power of Two
 */
static  bool ht_expand(struct ht* table){

    size_t new_size = table->capacity * 2;
    if(new_size < table->capacity){
        return false; // Overflow detected
    }
    // alloc new memory
    ht_entry* new_entries = calloc(new_size, sizeof (ht_entry));
    // check for error
    if(new_entries == NULL){
        return false;
    }
    // copy old into new
    for(size_t i =0; i<table->capacity; i++){
        ht_entry entry_to_copy = table->entries[i];
        if(entry_to_copy.key != NULL){
            ht_set_entry(new_entries, new_size, entry_to_copy.key, entry_to_copy.value, NULL);
        }
    }
    // free old
    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_size;
    return true;

}