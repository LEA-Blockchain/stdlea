#include "stddef.h"
#include "stdlea.h"
#include "string.h"
#include <stdint.h>

#ifndef DISABLE_BUMP_ALLOCATOR
/**
 * @brief The static memory heap for the LEA program.
 * @note The size is defined by LEA_HEAP_SIZE in stdlea.h.
 */
static uint8_t heap[LEA_HEAP_SIZE];

/**
 * @brief Pointer to the next available offset in the heap for the bump allocator.
 */
static size_t heap_top = 0;

LEA_EXPORT(__lea_malloc)
__attribute__((used)) void *malloc(size_t size) {
    if (size > LEA_HEAP_SIZE - heap_top)
        LEA_ABORT();

    void *ptr = &heap[heap_top];
    heap_top += size;
    return ptr;
}

LEA_EXPORT(__lea_allocator_reset)
__attribute__((used)) void allocator_reset() {
    // This will use our optimized `memset` above.
    memset(heap, 0, sizeof(heap));
    heap_top = 0;
}

/**
 * @brief Gets the base address of the heap. Exported for the host environment.
 */
LEA_EXPORT(__lea_get_heap_base)
__attribute__((used)) void *__lea_get_heap_base() {
    return heap;
}

/**
 * @brief Gets the current top of the heap (total bytes allocated). Exported for the host.
 */
LEA_EXPORT(__lea_get_heap_top)
__attribute__((used)) size_t __lea_get_heap_top() {
    return heap_top;
}

#endif // DISABLE_BUMP_ALLOCATOR
