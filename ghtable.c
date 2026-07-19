#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ghtable.h"

static inline key_list_entry* allocate_key_list(size_t capacity)
{
    key_list_entry* list = malloc(capacity*sizeof(key_list_entry));
    if ( !list )
        return NULL;

    return list;
}

static inline void free_key_list(key_list_entry* list)
{
    if ( !list )
        return;

    free(list);
}

ghtable* new_ghtable(size_t est_init_count, char type)
{
    if ( !est_init_count || ( type != ORD && type != UNORD ) )
        return NULL;

    ghtable* ght = calloc(1, sizeof *ght);
    if ( !ght )
        return NULL;

    ghtable_entry* table = calloc( 2*est_init_count, sizeof(ghtable_entry) );
    if ( !table )
    {
        free_ghtable(ght);
        return NULL;
    }

    ght->capacity = 2*est_init_count;
    ght->count = 0;
    ght->table = table;

    if ( type == ORD )
    {
        key_list_entry* list = allocate_key_list(est_init_count);
        if ( !list )
        {
            free_ghtable(ght);
            return NULL;
        }
        ght->keys = list;
        ght->key_list_capacity = est_init_count;
    }
    else
    {
        ght->keys = NULL;
        ght->key_list_capacity = 0;
    }

    return ght;
}

void free_ghtable(ghtable* ght)
{
    if ( !ght )
        return;

    ghtable_entry* table = ght->table;
    size_t capacity = ght->capacity;

    for ( size_t i = 0; i < capacity; i++ )
    {
        if ( table[i].key )
        {
            free(table[i].key);
            free(table[i].value);
        }
    }
    free(table);

    free_key_list(ght->keys);
    free(ght);
}

static inline size_t get_hash(const void* key, size_t n)
{
    size_t hash = 0;
    const unsigned char* key_ptr = (const unsigned char*)key;

    for ( size_t i = 0; i < n; i++ )
        hash = (hash*P + key_ptr[i]) % M;

    return hash;
}

ghtable_entry* ghtable_get_entry(ghtable* ght, const void* key, size_t key_size)
{
    size_t ght_capacity = ght->capacity;
    size_t hash = get_hash(key, key_size);
    size_t index = hash % ght_capacity;
    ghtable_entry* table = ght->table;
    const void* entry_key;

    while ( (entry_key = table[index].key) )
    {
        if ( table[index].key_len == key_size && !memcmp(entry_key, key, key_size) )
            return &table[index];

        if ( ++index == ght_capacity )
            index = 0;
    }

    return NULL;
}

void* ghtable_get(ghtable* ght, const char* key)
{
    ghtable_entry* entry;
    if ( (entry = ghtable_get_entry(ght, key, strlen(key))) )
        return entry->value;
    else
        return NULL;
}

void* ghtable_getn(ghtable* ght, const void* key, size_t key_size)
{
    ghtable_entry* entry;
    if ( (entry = ghtable_get_entry(ght, key, key_size)) )
        return entry->value;
    else
        return NULL;
}

void* ghtable_get_nth(ghtable* ght, size_t index)
{
    if ( !ght->keys || !ght->count )
        return NULL;

    if ( index > ght->count - 1 )
        return NULL;

    key_list_entry kl_entry = ght->keys[index];

    return ghtable_getn(ght, kl_entry.key, kl_entry.key_len);
}

void* ghtable_cv(ghtable* ght, const char* key, void* buffer)
{
    ghtable_entry* entry = ghtable_get_entry(ght, key, strlen(key));

    size_t value_size = entry->value_size;
    void* value = entry->value;


    return NULL;
}

static inline void rebuild_table(ghtable_entry* old_table, size_t old_capacity,
                                 ghtable_entry* new_table, size_t new_capacity)
{
    ghtable_entry entry;
    size_t index = 0;

    for ( size_t i = 0; i < old_capacity; i++ )
    {
        entry = old_table[i];
        if ( entry.key )
        {
            index = entry.hash % new_capacity;
            while ( new_table[index].key )
            {
                if ( ++index == new_capacity )
                    index = 0;
            }
            new_table[index] = entry;
        }
    }
}

ghtable* ghtable_grow(ghtable* ght, size_t factor)
{
    if ( factor < 2 )
        return NULL;

    size_t old_capacity = ght->capacity;
    size_t new_capacity = factor * old_capacity;

    ghtable_entry* new_table = calloc(new_capacity, sizeof(ghtable_entry));
    if ( !new_table )
        return NULL;

    ghtable_entry* old_table = ght->table;

    rebuild_table(old_table, old_capacity, new_table, new_capacity);

    ght->table = new_table;
    ght->capacity = new_capacity;

    free(old_table);

    return ght;
}

ghtable* ghtable_shrink(ghtable* ght)
{
    size_t old_capacity = ght->capacity;
    size_t new_capacity = (size_t)(ght->count / LOAD_FACTOR);

    ghtable_entry* new_table = calloc( new_capacity, sizeof(ghtable_entry) );
    if ( !new_table )
        return NULL;

    if ( ght->keys )
    {
        size_t kl_new_capacity = ght->count*sizeof(key_list_entry);
        key_list_entry* keys = ght->keys;

        keys = realloc( keys, kl_new_capacity );
        if ( !keys )
        {
            free(new_table);
            return NULL;
        }

        ght->keys = keys;
        ght->key_list_capacity = kl_new_capacity;
    }

    ghtable_entry* old_table = ght->table;

    rebuild_table(old_table, old_capacity, new_table, new_capacity);

    ght->table = new_table;
    ght->capacity = new_capacity;

    free(old_table);

    return ght;
}

void ghtable_drop_keylist(ghtable* ght)
{
    if ( ght->keys )
    {
        free(ght->keys);

        ght->key_list_capacity = 0;
        ght->keys = NULL;
    }
}

static inline key_list_entry* add_key(ghtable* ght, const void* key, size_t key_size)
{
    if ( ght->count == ght->key_list_capacity )
    {
        key_list_entry* list = realloc(ght->keys,
                                       2*ght->count*sizeof(key_list_entry));
        if ( !list )
            return NULL;

        ght->keys = list;
        ght->key_list_capacity = 2*ght->count;
    }

    ght->keys[ght->count] = (key_list_entry){(void*)key, key_size};

    return ght->keys;
}

static inline void del_key(ghtable* ght, const void* key, size_t key_size)
{
    key_list_entry* list = ght->keys;
    size_t ght_count = ght->count;

    size_t i = 0;
    while ( i < ght_count && memcmp(list[i].key, key, key_size) )
        i++;
    // [0, 1, 2, 3, 4, 5]
    memmove(list + i, list + i + 1, (ght_count - 1 - i)*sizeof(key_list_entry) );
    list[ght_count - 1] = (key_list_entry){NULL, 0};
}

static inline void remove_entry(ghtable_entry* entry)
{
    free(entry->key);
    free(entry->value);

    *entry = (ghtable_entry){NULL, NULL, 0, 0, 0};
}

void* ghtable_set(ghtable* ght, const char* key, void* value, size_t size)
{
    // size_t ght_capacity = ght->capacity;
    if ( ght->count > LOAD_FACTOR * ght->capacity )
    {
        if ( !ghtable_grow(ght, 2) )
            return NULL;
    }
    size_t ght_capacity = ght->capacity;

    size_t key_len = strlen(key);
    size_t hash = get_hash(key, key_len);
    size_t index = hash % ght_capacity;
    ghtable_entry* table = ght->table;
    char* entry_key;
    bool overwrite = false;

    while ( (entry_key = table[index].key) )
    {
        if ( !strcmp(entry_key, key) )
        {
            overwrite = true;
            break;
        }

        if ( ++index == ght_capacity )
            index = 0;
    }

    if ( !overwrite )
    {
        if ( !(table[index].key = strdup(key)) )
            return NULL;
    }

    void* value_ptr = malloc(size);
    if ( !value_ptr )
    {
        remove_entry(&table[index]);
        return NULL;
    }

    if ( overwrite )
        free(table[index].value);

    table[index].value = memcpy(value_ptr, value, size);
    table[index].hash = hash;
    table[index].key_len = key_len;
    table[index].value_size = size;

    if ( ght->keys && !overwrite )
    {
        if ( !add_key(ght, table[index].key, table[index].key_len) )
        {
            remove_entry(&table[index]);
            return NULL;
        }
    }

    if ( !overwrite )
        ght->count++;

    return value_ptr;
}

void* ghtable_setn(ghtable* ght, const void* key, size_t key_size, void* value, size_t value_size)
{
    if ( ght->count > LOAD_FACTOR * ght->capacity )
    {
        if ( !ghtable_grow(ght, 2) )
            return NULL;
    }
    size_t ght_capacity = ght->capacity;

    size_t hash = get_hash(key, key_size);
    size_t index = hash % ght_capacity;
    ghtable_entry* table = ght->table;
    void* entry_key;
    bool overwrite = false;

    while ( (entry_key = table[index].key) )
    {
        if ( table[index].key_len == key_size && !memcmp(entry_key, key, key_size) )
        {
            overwrite = true;
            break;
        }

        if ( ++index == ght_capacity )
            index = 0;
    }

    if ( !overwrite )
    {
        void* key_ptr = malloc(key_size);
        if ( !key_ptr )
            return NULL;

        table[index].key = memcpy(key_ptr, key, key_size);
    }

    void* value_ptr = malloc(value_size);
    if ( !value_ptr )
    {
        remove_entry(&table[index]);
        return NULL;
    }

    if ( overwrite )
        free(table[index].value);

    table[index].value = memcpy(value_ptr, value, value_size);
    table[index].hash = hash;
    table[index].key_len = key_size;
    table[index].value_size = value_size;

    if ( ght->keys && !overwrite )
    {
        if ( !add_key(ght, table[index].key, key_size) )
        {
            remove_entry(&table[index]);
            return NULL;
        }
    }

    if ( !overwrite )
        ght->count++;

    return value_ptr;
}

static inline void shift_entries(ghtable* ght, size_t index)
{
    size_t capacity = ght->capacity;
    ++index == capacity ? index = 0 : index;

    ghtable_entry* table = ght->table;
    const char* entry_key;
    size_t expected_index;

    while ( (entry_key = table[index].key) )
    {
        expected_index = table[index].hash % capacity;

        if ( index == expected_index )
            break;
        else if ( index == 0 )
            table[capacity - 1] = table[index];
        else
            table[index-1] = table[index];

        table[index] = (ghtable_entry){NULL, NULL, 0, 0, 0};

        if ( ++index == capacity )
            index = 0;
    }
}

int ghtable_del(ghtable* ght, const char* key)
{
    size_t key_len = strlen(key);
    ghtable_entry* entry = ghtable_get_entry(ght, key, key_len);

    if ( entry )
    {
        ghtable_entry* table = ght->table;
        size_t capacity = ght->capacity;
        size_t index = entry - table;

        remove_entry(entry);
        shift_entries(ght, index);

        if ( ght->keys )
            del_key(ght, key, key_len);

        ght->count--;
    }
    else
        return -1;

    return 0;
}

int ghtable_deln(ghtable* ght, const void* key, size_t key_len)
{
    ghtable_entry* entry = ghtable_get_entry(ght, key, key_len);
    if ( entry )
    {
        ghtable_entry* table = ght->table;
        size_t capacity = ght->capacity;
        size_t index = entry - table;

        remove_entry(entry);
        shift_entries(ght, index);

        if ( ght->keys )
            del_key(ght, key, key_len);

        ght->count--;
    }
    else
        return -1;

    return 0;
}
