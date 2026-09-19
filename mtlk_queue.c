#include <stddef.h>
#include <stdlib.h>

#define que_PUSH(queue_ptr, type, item)                     \
do                                                          \
{                                                           \
    mtlk_queue* queue = (queue_ptr);                        \
    size_t cursor = queue->cursor;                          \
                                                            \
    ((type*)queue->buffer)[cursor] = (item);                \
                                                            \
    if ( queue->count < queue->capacity )                   \
        queue->count++;                                     \
                                                            \
    queue->cursor = ++cursor % queue->capacity;             \
                                                            \
} while (0);

#define que_GET(queue_ptr, type, index)                     \
*(type*)mtlk_queue_get(queue_ptr, index, sizeof(type))

typedef struct
{
    size_t capacity;
    size_t count;
    size_t cursor;
    void* buffer;

} mtlk_queue;

const mtlk_queue INVALID_QUEUE = (mtlk_queue){0};

// 0,0,0 -> 1,0,0 -> 1,2,0 -> 1,2,3
// 1,2,3 -> 4,2,3 -> 4,5,3 -> 4,5,6  // curs + idx
// [1,2,3]-[4,2,3]-[4,5,3]-[4,5,6]

mtlk_queue new_mtlk_queue(size_t capacity, size_t count, void* buffer)
{
    if ( capacity <= 1 || count > capacity || !buffer )
        return INVALID_QUEUE;

    return (mtlk_queue)
           {
                .capacity = capacity,
                .count = count,
                .cursor = 0,
                .buffer = buffer
           };
}

void* mtlk_queue_get(mtlk_queue* queue, size_t index, size_t unit_size)
{
    index = ((queue->cursor+index) % queue->capacity)*unit_size;
    return (unsigned char*)queue->buffer + index;
}
