#ifndef STDLIB_H
#define STDLIB_H

#include "stddef.h"

/**
 * @brief Allocates memory from the heap.
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory.
 * @warning This is a simple bump allocator. No `free` is available.
 */
void *malloc(size_t size);

/**
 * @def abort()
 * @brief Aborts program execution by causing a trap.
 */
#define abort() __builtin_trap()

/**
 * @def free(x)
 * @brief Intentionally triggers a compile-time error if `free()` is used.
 * @param x The pointer to "free".
 * @note LEA uses a bump allocator and does not support `free()`.
 *       Use `allocator_reset()` to clear the entire heap.
 */
#define free(x)                                                                                    \
    _Static_assert(0, "free() is not supported. LEA uses a bump allocator. Call "                  \
                      "allocator_reset() instead.");

#endif // STDLIB_H
