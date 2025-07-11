#ifndef STDDEF_H
#define STDDEF_H
#include "feature_check.h"

// size_t: unsigned integer type used for sizes
typedef unsigned int size_t;

// ptrdiff_t: signed integer type for pointer differences
typedef int ptrdiff_t;

// NULL: null pointer constant
#ifndef NULL
#define NULL ((void *)0)
#endif

// offsetof: macro to get byte offset of a member in a struct
#define offsetof(type, member) ((size_t) & (((type *)0)->member))

// Static asserts to confirm sizes are correct for 32-bit WASM
_Static_assert(sizeof(size_t) == 4, "size_t must be 4 bytes (WASM 32-bit)");
_Static_assert(sizeof(ptrdiff_t) == 4, "ptrdiff_t must be 4 bytes (WASM 32-bit)");

#endif // _STDDEF_H
