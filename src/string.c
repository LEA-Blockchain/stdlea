#include "string.h"
#include "stddef.h"
#include "stdlea.h"
#include <stdint.h>

// --- Standard Library Memory Functions ---
void *memset(void *dest, int val, size_t len) {
    unsigned char *ptr = dest;
    unsigned char c = (unsigned char)val;
    for (size_t i = 0; i < len; i++) {
        ptr[i] = c;
    }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t len) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    for (size_t i = 0; i < len; i++) {
        d[i] = s[i];
    }
    return dest;
}

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

size_t strlen(const char *s) {
    size_t i = 0;
    while (s[i]) {
        i++;
    }
    return i;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

char *strcpy(char *dest, const char *src) {
    char *orig = dest;
    while ((*dest++ = *src++))
        ;
    return orig;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0) {
        return 0;
    }
    do {
        if (*s1 != *s2++) {
            return *(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1);
        }
        if (*s1++ == 0) {
            break;
        }
    } while (--n != 0);
    return 0;
}

size_t strnlen(const char *s, size_t maxlen) {
    size_t i = 0;
    while (i < maxlen && s[i]) {
        i++;
    }
    return i;
}
