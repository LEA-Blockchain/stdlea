#ifndef STDIO_H
#define STDIO_H
#include "stddef.h"

/**
 * @brief Explanation of the Format String (`fmt`) parameter.
 *
 * The format string is a null-terminated character string that dictates the
 * content and layout of the final output. It is composed of two types of items:
 * 1. Literal Characters: These are copied verbatim to the output.
 * 2. Format Specifiers: Each begins with a '%' and is replaced by a value
 * from the variadic arguments passed to the function.
 *
 * Example:
 * lea_printf("VM: %s, ID: %d", "LEA-VM", 123);
 * Output: "VM: LEA-VM, ID: 123"
 *
 * --- Supported Format Specifiers ---
 * %d, %i : Signed decimal integer.
 * %u     : Unsigned decimal integer.
 * %s     : Null-terminated string of characters.
 * %c     : Single character.
 * %x     : Unsigned hexadecimal integer (lowercase).
 * %b     : Unsigned binary integer (custom specifier).
 * %%     : A literal percent sign '%' is printed.
 *
 * --- Length Modifiers (placed before the specifier) ---
 * h      : Argument is treated as a 'short'. (e.g., %hd, %hx).
 * hh     : Argument is treated as a 'char'. (e.g., %hhd, %hhx).
 *
 * --- Arbitrary-Length Specifiers ---
 * %*x    : Prints a blob of memory as a hex string. Requires two arguments:
 * (size_t length, const unsigned char* data).
 * %*b    : Prints a blob of memory as a binary string. Requires two arguments:
 * (size_t length, const unsigned char* data).
 * %*s    : Prints a string with a specified length, not null-terminated.
 * Requires two arguments: (size_t length, const char* string).
 */

void lea_printf(const char *fmt, ...);

int lea_snprintf(char *buffer, size_t size, const char *fmt, ...);

#define printf lea_printf
#define snprintf lea_snprintf

#endif // STDIO_H
