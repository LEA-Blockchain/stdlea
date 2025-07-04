#include "stdlea.h"

/**
 * @brief The static memory heap for the Lea program.
 * @note The size is defined by LEA_HEAP_SIZE in stdlea.h.
 */
static uint8_t heap[LEA_HEAP_SIZE];

/**
 * @brief Pointer to the next available free memory block in the heap.
 */
static size_t heap_top = 0;

#ifdef __wasm_bulk_memory__
/**
 * @brief Fills a block of memory with a specified value.
 * @param dest Pointer to the block of memory to fill.
 * @param val Value to be set. The value is passed as an int, but the function
 *            fills the block of memory using the unsigned char conversion of this value.
 * @param len Number of bytes to be set to the value.
 * @return A pointer to the memory area `dest`.
 * @note This version uses the compiler's `__builtin_memset` for optimization
 *       when bulk memory operations are available.
 */
void *memset(void *dest, int val, size_t len)
{
    // __builtin_memset might be optimized using memory.fill
    return __builtin_memset(dest, val, len);
}
#else
/**
 * @brief Fills a block of memory with a specified value.
 * @param dest Pointer to the block of memory to fill.
 * @param val Value to be set. The value is passed as an int, but the function
 *            fills the block of memory using the unsigned char conversion of this value.
 * @param len Number of bytes to be set to the value.
 * @return A pointer to the memory area `dest`.
 * @note This is a fallback implementation for environments without bulk memory support.
 */
void *memset(void *dest, int val, size_t len)
{
    unsigned char *ptr = (unsigned char *)dest;
    unsigned char byte_val = (unsigned char)val;
    size_t i;
    for (i = 0; i < len; ++i)
    {
        ptr[i] = byte_val;
    }
    return dest;
}
#endif

#ifdef __wasm_bulk_memory__
/**
 * @brief Copies a block of memory from a source location to a destination location.
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src Pointer to the source of data to be copied.
 * @param len Number of bytes to copy.
 * @return A pointer to the destination, which is `dest`.
 * @note This version uses the compiler's `__builtin_memcpy` for optimization
 *       when bulk memory operations are available. Assumes no overlap between src and dest.
 */
void *memcpy(void *dest, const void *src, size_t len)
{
    return __builtin_memcpy(dest, src, len);
}
#else
/**
 * @brief Copies a block of memory from a source location to a destination location.
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src Pointer to the source of data to be copied.
 * @param len Number of bytes to copy.
 * @return A pointer to the destination, which is `dest`.
 * @note This is a fallback implementation for environments without bulk memory support.
 *       Assumes no overlap between src and dest.
 */
void *memcpy(void *dest, const void *src, size_t len)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    size_t i;
    for (i = 0; i < len; ++i)
    {
        d[i] = s[i];
    }
    return dest;
}
#endif

#ifdef __wasm_bulk_memory__
/**
 * @brief Moves a block of memory from a source location to a destination location.
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src Pointer to the source of data to be copied.
 * @param len Number of bytes to copy.
 * @return A pointer to the destination, which is `dest`.
 * @note This version uses the compiler's `__builtin_memmove` for optimization
 *       when bulk memory operations are available. Handles overlapping memory regions correctly.
 */
void *memmove(void *dest, const void *src, size_t len)
{
    return __builtin_memmove(dest, src, len);
}
#else
/**
 * @brief Moves a block of memory from a source location to a destination location.
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src Pointer to the source of data to be copied.
 * @param len Number of bytes to copy.
 * @return A pointer to the destination, which is `dest`.
 * @note This is a fallback implementation for environments without bulk memory support.
 *       Handles overlapping memory regions correctly.
 */
void *memmove(void *dest, const void *src, size_t len)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    size_t i;

    if (d == s || len == 0)
    {
        return dest;
    }

    if (d < s || d >= (s + len))
    {
        for (i = 0; i < len; ++i)
        {
            d[i] = s[i];
        }
    }
    else
    {
        for (i = len; i > 0; --i)
        {
            d[i - 1] = s[i - 1];
        }
    }
    return dest;
}
#endif

/**
 * @fn void *malloc(size_t size)
 * @brief Allocates a block of memory from the static heap.
 * @return A pointer to the allocated memory block.
 * @warning This is a simple bump allocator. There is no `free` function.
 *          The program will abort if allocation fails.
 */
LEA_EXPORT(__lea_malloc) __attribute__((used)) void *malloc(size_t size)
{
    if (size > LEA_HEAP_SIZE - heap_top)
    {
        lea_abort("Out of memory in malloc (overflow-safe)");
    }

    void *ptr = &heap[heap_top];
    heap_top += size;
    return ptr;
}

/**
 * @brief Resets the heap allocator.
 * @note This function clears the entire heap to zero and resets the heap pointer
 *       to the beginning of the heap.
 */
LEA_EXPORT(__lea_allocator_reset)
__attribute__((used)) void allocator_reset()
{
    memset(heap, 0, sizeof(heap)); // Reset the heap
    heap_top = 0;                  // Reset the heap pointer
    return;
}

/**
 * @brief Gets the base address of the heap.
 * @return A pointer to the beginning of the static heap.
 * @note This function is exported to the host environment.
 */
LEA_EXPORT(__lea_get_heap_base)
__attribute__((used)) void *__lea_get_heap_base()
{
    return heap;
}

/**
 * @brief Gets the current top of the heap.
 * @return The current value of the heap_top offset.
 * @note This function is exported to the host environment. It represents the
 *       total number of bytes allocated.
 */
LEA_EXPORT(__lea_get_heap_top)
__attribute__((used)) size_t __lea_get_heap_top()
{
    return heap_top;
}
