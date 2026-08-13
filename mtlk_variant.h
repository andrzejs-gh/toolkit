#ifndef MTLK_VARIANT_H
#define MTLK_VARIANT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define V(type, val) (mtlk_variant){type, .type = val}
#define MKVOID(x) (x.type = VOID)
#define ISVOID(x) (x.type == VOID)

typedef enum
{
    // chars
    CHAR,
    UNSIGNED_CHAR,
    SIGNED_CHAR,
    // signed ints
    SHORT,
    INT,
    LONG,
    LONG_LONG,
    // signed ints exact
    INT8_T,
    INT16_T,
    INT32_T,
    INT64_T,
    // unsigned ints
    UNSIGNED_SHORT,
    UNSIGNED_INT,
    UNSIGNED_LONG,
    UNSIGNED_LONG_LONG,
    // unsigned ints exact
    UINT8_T,
    UINT16_T,
    UINT32_T,
    UINT64_T,
    // size_t
    SIZE_T,
    // bool
    BOOL,
    // floats
    FLOAT,
    DOUBLE,
    LONG_DOUBLE,
    // pointer
    PTR,

    // void
    VOID

} mtlk_variant_type;

typedef struct
{
    mtlk_variant_type type;
    union
    {
        // chars
        char CHAR;
        unsigned char UNSIGNED_CHAR;
        signed char SIGNED_CHAR;
        // signed ints
        short SHORT;
        int INT;
        long LONG;
        long long LONG_LONG;
        // signed ints exact
        int8_t INT8_T;
        int16_t INT16_T;
        int32_t INT32_T;
        int64_t INT64_T;
        // unsigned ints
        unsigned short UNSIGNED_SHORT;
        unsigned int UNSIGNED_INT;
        unsigned long UNSIGNED_LONG;
        unsigned long long UNSIGNED_LONG_LONG;
        // unsigned ints exact
        uint8_t UINT8_T;
        uint16_t UINT16_T;
        uint32_t UINT32_T;
        uint64_t UINT64_T;
        // size_t
        size_t SIZE_T;
        // bool
        bool BOOL;
        // floats
        float FLOAT;
        double DOUBLE;
        long double LONG_DOUBLE;
        // pointer
        void* PTR;
    };

} mtlk_variant;


#endif
