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
    const void* key;
    size_t key_len;

} key_list_entry;

typedef struct
{
    const void* value;
    size_t size;

} value_view;

typedef struct ghtable ghtable;
typedef struct ghtable_entry ghtable_entry;

ghtable* new_ghtable(size_t est_init_count, char type);
void free_ghtable(ghtable* ght);
const key_list_entry* ghtable_key_list(ghtable* ght);

size_t ghtable_capacity(ghtable* ght);
size_t ghtable_count(ghtable* ght);

size_t ghtable_size(ghtable* ght);
size_t ghtable_shrink_size(ghtable* ght);
size_t ghtable_key_list_size(ghtable* ght);
size_t ghtable_key_list_shrink_size(ghtable* ght);

ghtable* ghtable_grow(ghtable* ght, size_t factor);
ghtable* ghtable_shrink(ghtable* ght);

const void* ghtable_get(ghtable* ght, const char* key);
const void* ghtable_getn(ghtable* ght, const void* key, size_t key_size);
value_view ghtable_view(ghtable* ght, const char* key);
value_view ghtable_viewn(ghtable* ght, const void* key, size_t key_len);

const void* ghtable_set(ghtable* ght, const char* key, void* value, size_t size);
const void* ghtable_setn(ghtable* ght, const void* key, size_t key_size, void* value, size_t value_size);

int ghtable_del(ghtable* ght, const char* key);
int ghtable_deln(ghtable* ght, const void* key, size_t key_len);

const void* ghtable_nth(ghtable* ght, size_t index);
const void* ghtable_nth_key(ghtable* ght, size_t index);
key_list_entry ghtable_nth_kl_entry(ghtable* ght, size_t index);

void ghtable_drop_keylist(ghtable* ght);

void* ghtable_cv(ghtable* ght, const char* key, void* buffer);
void* ghtable_cvn(ghtable* ght, const void* key, size_t key_len, void* buffer);

#endif
