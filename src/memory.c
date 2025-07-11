#include "stddef.h"
#include "stdlea.h"
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

// --- Standard Library Memory Functions ---
/**
 * @brief Fills a block of memory with a specific value.
 * @param dest Pointer to the block of memory to fill.
 * @param val The value to be set.
 * @param len The number of bytes to be set to the value.
 * @return A pointer to the memory area `dest`.
 */
void *memset(void *dest, int val, size_t len) {
    unsigned char *ptr = dest;
    unsigned char c = (unsigned char)val;
    for (size_t i = 0; i < len; i++) {
        ptr[i] = c;
    }
    return dest;
}

/**
 * @brief Copies a block of memory from a source to a destination.
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src Pointer to the source of data to be copied.
 * @param len The number of bytes to copy.
 * @return A pointer to the destination.
 * @note The memory areas should not overlap. Use memmove for overlapping memory.
 */
void *memcpy(void *dest, const void *src, size_t len) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    for (size_t i = 0; i < len; i++) {
        d[i] = s[i];
    }
    return dest;
}

/**
 * @brief Moves a block of memory from a source to a destination, handling overlapping memory.
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src Pointer to the source of data to be copied.
 * @param len The number of bytes to move.
 * @return A pointer to the destination.
 */
void *memmove(void *dest, const void *src, size_t len) {
    unsigned char *d = dest;
    const unsigned char *s = src;

    if (d == s) {
        // Source and destination are the same, no-op.
        return dest;
    }

    if (d < s) {
        // Destination is before the source, so a forward copy is safe.
        for (size_t i = 0; i < len; i++) {
            d[i] = s[i];
        }
    } else {
        // Destination is after the source, so a backward copy is required
        // to prevent overwriting data before it's been copied.
        for (size_t i = len; i != 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }
    return dest;
}

/**
 * @brief Compares two blocks of memory.
 * @param s1 Pointer to the first block of memory.
 * @param s2 Pointer to the second block of memory.
 * @param n The number of bytes to compare.
 * @return An integer less than, equal to, or greater than zero if the first `n` bytes of `s1`
 * is found, respectively, to be less than, to match, or be greater than the first `n` bytes of
 * `s2`.
 */
int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = s1;
    const unsigned char *p2 = s2;
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            // Return the difference of the first non-matching bytes
            return p1[i] - p2[i];
        }
    }
    return 0; // The memory regions are identical
}

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
