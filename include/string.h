#ifndef STRING_H
#define STRING_H

#include "stddef.h"

/**
 * @brief Fills a block of memory with a specific value.
 * @param dest Pointer to the block of memory to fill.
 * @param val The value to be set.
 * @param len The number of bytes to be set to the value.
 * @return A pointer to the memory area `dest`.
 */
void *memset(void *dest, int val, size_t len);

/**
 * @brief Copies a block of memory from a source to a destination.
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src Pointer to the source of data to be copied.
 * @param len The number of bytes to copy.
 * @return A pointer to the destination.
 * @note The memory areas should not overlap. Use memmove for overlapping memory.
 */
void *memcpy(void *dest, const void *src, size_t len);

/**
 * @brief Moves a block of memory from a source to a destination, handling overlapping memory.
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src Pointer to the source of data to be copied.
 * @param len The number of bytes to move.
 * @return A pointer to the destination.
 */
void *memmove(void *dest, const void *src, size_t len);

/**
 * @brief Compares two blocks of memory.
 * @param s1 Pointer to the first block of memory.
 * @param s2 Pointer to the second block of memory.
 * @param n The number of bytes to compare.
 * @return An integer less than, equal to, or greater than zero if the first `n` bytes of `s1`
 * is found, respectively, to be less than, to match, or be greater than the first `n` bytes of
 * `s2`.
 */
int memcmp(const void *s1, const void *s2, size_t n);

/**
 * @brief Calculates the length of a null-terminated string.
 * @param s The null-terminated string to be measured.
 * @return The number of characters in the string, excluding the null-terminator.
 */
size_t strlen(const char *s);

/**
 * @brief Compares two null-terminated strings lexicographically.
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * @return An integer less than, equal to, or greater than zero if `s1` is found,
 * respectively, to be less than, to match, or be greater than `s2`.
 */
int strcmp(const char *s1, const char *s2);

/**
 * @brief Compares up to `n` characters of two null-terminated strings.
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * @param n The maximum number of characters to compare.
 * @return An integer less than, equal to, or greater than zero if the first `n` bytes of `s1`
 * is found, respectively, to be less than, to match, or be greater than the first `n` bytes of
 * `s2`.
 */
int strncmp(const char *s1, const char *s2, size_t n);

/**
 * @brief Copies a null-terminated string from a source to a destination.
 * @param dest Pointer to the destination buffer.
 * @param src Pointer to the source string.
 * @return A pointer to the destination string `dest`.
 * @note The destination buffer must be large enough to hold the source string,
 *       including the null-terminator. The behavior is undefined if the strings
 *       overlap.
 */
char *strcpy(char *dest, const char *src);

/**
 * @brief Calculates the length of a string up to a maximum size.
 * @param s The null-terminated string.
 * @param maxlen The maximum number of characters to check.
 * @return The number of characters in the string, excluding the null-terminator,
 *         or `maxlen` if the null-terminator is not found in the first `maxlen` characters.
 */
size_t strnlen(const char *s, size_t maxlen);

#endif
