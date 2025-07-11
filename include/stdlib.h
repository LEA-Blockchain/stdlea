#ifndef STDLIB_H
#define STDLIB_H
#include "feature_check.h"

#include <stddef.h> // for size_t

/**
 * @brief Allocates memory from the heap.
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory.
 * @warning This is a simple bump allocator. No `free` is available.
 */
void *malloc(size_t size);

/**
 * @brief Aborts the program.
 */
#define abort() __builtin_trap()

/**
 * @brief Intentionally trigger an error if `free` is used.
 */
#define free(x)                                                                                    \
    _Static_assert(0, "free() is not supported. LEA uses a bump allocator. Call "                  \
                      "allocator_reset() instead.");

#endif // STDLIB_H
