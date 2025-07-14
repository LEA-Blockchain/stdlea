#ifndef STDIO_H
#define STDIO_H
#include "stddef.h"
#include <stdarg.h>

#ifdef ENABLE_LEA_FMT
/**
 * @brief Prints a formatted string to the host environment.
 * @param fmt The format string.
 * @param ... Variable arguments to be formatted.
 * @note This function buffers output and sends it to the host via `__lea_log2`.
 *       It is intended for general logging. For format details, see snprintf().
 */
void printf(const char *fmt, ...);

/**
 * @brief Writes a formatted string to a buffer.
 *
 * @param buffer Pointer to the destination buffer where the resulting string is stored.
 * @param size The maximum number of characters to be written to the buffer,
 *             including the null-terminator.
 * @param fmt The format string, which contains literal characters and format specifiers.
 * @param ... Variable arguments to be formatted according to the specifiers in `fmt`.
 *
 * @return The total number of characters that *would have been* written if the buffer
 *         were large enough, not including the null-terminator. A return value
 *         greater than or equal to `size` indicates that the output was truncated.
 *
 * @note **Format Specifiers:**
 *       - `%d`, `%i`: Signed decimal integer (`int`, `short`, `char`, `long long`).
 *       - `%u`: Unsigned decimal integer (`unsigned int`, `unsigned short`, etc.).
 *       - `%x`: Unsigned hexadecimal integer (lowercase).
 *       - `%b`: Unsigned binary integer.
 *       - `%s`: Null-terminated string. A `NULL` pointer is printed as `(null)`.
 *       - `%c`: Single character.
 *       - `%%`: A literal percent sign.
 *
 * @note **Length Modifiers:**
 *       - `h`: Argument is a `short` or `unsigned short`.
 *       - `hh`: Argument is a `signed char` or `unsigned char`.
 *       - `ll`: Argument is a `long long` or `unsigned long long`.
 *
 * @note **Arbitrary-Length Specifiers:**
 *       - `%*x`: Hex blob. Requires `(size_t, const unsigned char*)`.
 *       - `%*b`: Binary blob. Requires `(size_t, const unsigned char*)`.
 *       - `%*s`: Sized string. Requires `(size_t, const char*)`.
 */
int snprintf(char *buffer, size_t size, const char *fmt, ...);

/**
 * @brief Writes a formatted string to a buffer, processing a va_list of arguments.
 *
 * This function is the core implementation for `snprintf`-like functionality.
 * @param buffer Pointer to the destination buffer.
 * @param size The maximum number of characters to write.
 * @param fmt The format string.
 * @param args A `va_list` of arguments.
 * @return The total number of characters that would have been written.
 */
int vsnprintf(char *buffer, size_t size, const char *fmt, va_list args);

#else

#define printf(...)
#define snprintf(...)
#define vsnprintf(...)

#endif // ENABLE_LEA_FMT

#endif // STDIO_H
