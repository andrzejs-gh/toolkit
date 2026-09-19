#include <stddef.h>
#include <stdlib.h>

#define que_PUSH_VAL(queue_ptr, type, item)                 \
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
    queue->cursor = ++cursor % (queue->capacity - 1);       \
                                                            \
} while (0);

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
    if ( !capacity || !count || !buffer )
        return INVALID_QUEUE;

    return (mtlk_queue)
           {
                .capacity = capacity,
                .count = count,
                .cursor = 0,
                .buffer = buffer
           };
}

void push_by_value()
{
    do
    {

    } while (0);
}

// void push(int_queue* que, int item)
// {
//     size_t cursor = que->cursor;
//     que->buffer[cursor] = item;
//
//     if ( cursor < que->capacity - 1 )
//     {
//         que->cursor++;
//     }
//     else
//     {
//         que->cursor = 0;
//     }
// }
//
// int get(int_queue* que, size_t index)
// {
//     return que->buffer[(que->cursor+index) % que->capacity];
// }
