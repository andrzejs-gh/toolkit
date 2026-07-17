#ifndef GHTABLE_LIB_H
#define GHTABLE_LIB_H

#include <stddef.h>

#define P 31
#define M 1000000007
#define LOAD_FACTOR 0.7

#define ORD 'o'
#define UNORD 'u'

typedef struct
{
    void* key;
    size_t key_len;

} key_list_entry;

typedef struct
{
    key_list_entry* list;
    size_t capacity;

} key_list;

typedef struct
{
    void* key;
    void* value;
    size_t key_len;
    size_t value_size;
    size_t hash;

} ghtable_entry;

typedef struct
{
    size_t count;
    size_t capacity;
    ghtable_entry* table;
    key_list* keys;

} ghtable;

ghtable* new_ghtable(size_t initial_capacity, char type);
void free_ghtable(ghtable* ght);
ghtable_entry* ghtable_get_entry(ghtable* ght, const void* key, size_t key_size);
void* ghtable_get(ghtable* ght, const char* key);
void* ghtable_getn(ghtable* ght, const void* key, size_t key_size);
void* ghtable_get_nth(ghtable* ght, size_t index);
ghtable* ghtable_grow(ghtable* ght, size_t factor);
ghtable* ghtable_shrink(ghtable* ght);
void* ghtable_set(ghtable* ght, const char* key, void* value, size_t size);
void* ghtable_setn(ghtable* ght, const void* key, size_t key_size, void* value, size_t value_size);
int ghtable_del(ghtable* ght, const char* key);
int ghtable_deln(ghtable* ght, const void* key, size_t key_len);

#endif
