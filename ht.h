// Hashtable for the routes
#ifndef HTTPCSERVER_HT_H
#define HTTPCSERVER_HT_H

#include <stdbool.h>

typedef struct{
    char* key;
    void* value; // void pointer could point to everything
} ht_entry;

struct ht {
    ht_entry *entries;  // holds the entries array
    size_t capacity;    // size of entries array
    size_t length;     // current number of elements capacity >= length
};

struct ht* ht_create(void);
void ht_destroy(struct ht* table);
static bool ht_expand(struct ht* table);
const char* ht_set(struct ht* table, const char* key, void* value);
static const char* ht_set_entry(ht_entry* entries, size_t capacity, const char* key, void* value, size_t* p_length);
void* ht_get(struct ht* table, const char* key);

#endif //HTTPCSERVER_HT_H
