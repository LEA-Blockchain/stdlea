#include "string.h"
#include "stddef.h"
#include "stdlea.h"
#include <stdint.h>

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
 * @brief Calculates the length of a null-terminated string.
 * @param s The null-terminated string to be measured.
 * @return The number of characters in the string, excluding the null-terminator.
 */
size_t strlen(const char *s) {
    size_t i = 0;
    while (s[i]) {
        i++;
    }
    return i;
}
