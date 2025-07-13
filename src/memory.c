#include "stddef.h"
#include "stdlea.h"
#include "string.h"
#include <stdint.h>

// --- Heap Implementation ---

/**
 * @brief The static memory heap for the LEA program.
 * @note The size is defined by LEA_HEAP_SIZE in stdlea.h.
 */
static uint8_t heap[LEA_HEAP_SIZE];

/**
 * @brief Pointer to the next available offset in the heap for the bump allocator.
 */
static size_t heap_top = 0;

/**
 * @brief Allocates a block of memory from the static heap.
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory.
 * @warning This is a simple bump allocator. There is no `free` function.
 * The program will abort if the heap is out of memory.
 */
LEA_EXPORT(__lea_malloc)
__attribute__((used)) void *malloc(size_t size) {
    if (size > LEA_HEAP_SIZE - heap_top) {
        lea_abort("Out of memory");
    }

    void *ptr = &heap[heap_top];
    heap_top += size;
    return ptr;
}

/**
 * @brief Resets the heap allocator for reuse.
 * @note Clears the entire heap to zero and resets the heap allocation pointer.
 */
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
