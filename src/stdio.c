#include "stdio.h"
#include "stddef.h"
#include "stdlea.h"
#include <stdarg.h>

LEA_IMPORT(env, __lea_log2) void __lea_log2(const char *, size_t);

// Forward declarations for helpers
typedef struct printf_ctx_s printf_ctx_t;
static void snprintf_append_hex_blob(char **p_curr, char *end, const unsigned char *data,
                                     size_t len);
static void snprintf_append_binary_blob(char **p_curr, char *end, const unsigned char *data,
                                        size_t len);
static void snprintf_append_sized_string(char **p_curr, char *end, const char *s, size_t len);
static void ctx_append_hex_blob(printf_ctx_t *ctx, const unsigned char *data, size_t len);
static void ctx_append_binary_blob(printf_ctx_t *ctx, const unsigned char *data, size_t len);
static void ctx_append_sized_string(printf_ctx_t *ctx, const char *s, size_t len);

/**
 * @brief Appends a single character to the snprintf buffer.
 * @param p_curr Pointer to the current position in the buffer.
 * @param end Pointer to the end of the buffer.
 * @param c The character to append.
 */
static void snprintf_append_char(char **p_curr, char *end, char c) {
    if (*p_curr < end) {
        **p_curr = c;
    }
    (*p_curr)++;
}

/**
 * @brief Appends a null-terminated string to the snprintf buffer.
 * @param p_curr Pointer to the current position in the buffer.
 * @param end Pointer to the end of the buffer.
 * @param s The string to append.
 */
static void snprintf_append_string(char **p_curr, char *end, const char *s) {
    if (!s)
        s = "(null)";
    while (*s) {
        snprintf_append_char(p_curr, end, *s++);
    }
}

/**
 * @brief Appends an unsigned integer to the snprintf buffer in a given base.
 * @param p_curr Pointer to the current position in the buffer.
 * @param end Pointer to the end of the buffer.
 * @param n The unsigned integer to append.
 * @param base The base to use for representation (e.g., 10 for decimal, 16 for hex).
 */
static void snprintf_print_unsigned(char **p_curr, char *end, unsigned int n, unsigned int base) {
    char buf[33];
    const char *digits = "0123456789abcdef";
    int i = 0;
    if (base < 2 || base > 16)
        return;
    if (n == 0) {
        snprintf_append_char(p_curr, end, '0');
        return;
    }
    while (n > 0) {
        buf[i++] = digits[n % base];
        n /= base;
    }
    while (i-- > 0) {
        snprintf_append_char(p_curr, end, buf[i]);
    }
}

/**
 * @brief Writes a formatted string to a buffer, based on a format string and a va_list of
 * arguments.
 * @param buffer Pointer to the buffer where the resulting string is stored.
 * @param size The maximum number of characters to be written to the buffer.
 * @param fmt The format string that specifies how to interpret the data.
 * @param args A va_list of arguments to be formatted.
 * @return The number of characters that would have been written if the buffer were large enough,
 *         not including the null-terminator.
 * @note Supports the following format specifiers: %d, %i, %u, %x, %b, %s, %c, %%.
 *       Length modifiers `h` and `hh` are supported for integer types.
 *       The `*` specifier is supported for variable-length string and blob printing (e.g., %*x).
 */
int lea_vsnprintf(char *buffer, size_t size, const char *fmt, va_list args) {
    char *curr = buffer;
    char *end = (size > 0) ? (buffer + size - 1) : buffer;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            // --- Parse Length Modifiers ---
            int len_mod = 0; // 0=int, 1=h, 2=hh
            if (*fmt == 'h') {
                len_mod = 1;
                fmt++;
                if (*fmt == 'h') {
                    len_mod = 2;
                    fmt++;
                }
            }

            // --- Handle Specifiers ---
            if (*fmt == '*') {
                fmt++;
                size_t len = va_arg(args, size_t);
                switch (*fmt) {
                case 'x':
                    snprintf_append_hex_blob(&curr, end, va_arg(args, const unsigned char *), len);
                    break;
                case 'b':
                    snprintf_append_binary_blob(&curr, end, va_arg(args, const unsigned char *),
                                                len);
                    break;
                case 's':
                    snprintf_append_sized_string(&curr, end, va_arg(args, const char *), len);
                    break;
                }
            } else {
                switch (*fmt) {
                case 'd':
                case 'i': {
                    long long val; // Use long long to handle all cases before printing
                    if (len_mod == 2)
                        val = (signed char)va_arg(args, int);
                    else if (len_mod == 1)
                        val = (short)va_arg(args, int);
                    else
                        val = va_arg(args, int);

                    if (val < 0) {
                        snprintf_append_char(&curr, end, '-');
                        snprintf_print_unsigned(&curr, end, (unsigned int)-val, 10);
                    } else {
                        snprintf_print_unsigned(&curr, end, (unsigned int)val, 10);
                    }
                    break;
                }
                case 'u':
                case 'x':
                case 'b': {
                    unsigned int uval;
                    if (len_mod == 2)
                        uval = (unsigned char)va_arg(args, unsigned int);
                    else if (len_mod == 1)
                        uval = (unsigned short)va_arg(args, unsigned int);
                    else
                        uval = va_arg(args, unsigned int);

                    unsigned int base = (*fmt == 'x') ? 16 : ((*fmt == 'b') ? 2 : 10);
                    snprintf_print_unsigned(&curr, end, uval, base);
                    break;
                }
                case 's':
                    snprintf_append_string(&curr, end, va_arg(args, const char *));
                    break;
                case 'c':
                    snprintf_append_char(&curr, end, (char)va_arg(args, int));
                    break;
                case '%':
                    snprintf_append_char(&curr, end, '%');
                    break;
                default:
                    snprintf_append_char(&curr, end, '%');
                    snprintf_append_char(&curr, end, *fmt);
                    break;
                }
            }
        } else {
            snprintf_append_char(&curr, end, *fmt);
        }
        fmt++;
    }

    if (size > 0) {
        *curr = '\0';
    }
    return curr - buffer;
}

/**
 * @brief Writes a formatted string to a buffer.
 * @param buffer Pointer to the buffer where the resulting string is stored.
 * @param size The maximum number of characters to be written to the buffer.
 * @param fmt The format string.
 * @param ... Variable arguments to be formatted.
 * @return The number of characters that would have been written if the buffer were large enough.
 */
int lea_snprintf(char *buffer, size_t size, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = lea_vsnprintf(buffer, size, fmt, args);
    va_end(args);
    return ret;
}

/**
 * @brief Context structure for the buffered `lea_printf` implementation.
 * @note This is used internally by `lea_printf`.
 */
struct printf_ctx_s {
    char *buffer;
    const size_t size;
    size_t pos;
};

/**
 * @brief Flushes the printf context buffer to the host.
 * @param ctx The printf context.
 */
static void ctx_flush(printf_ctx_t *ctx) {
    if (ctx->pos > 0) {
        __lea_log2(ctx->buffer, ctx->pos);
        ctx->pos = 0;
    }
}

/**
 * @brief Appends a character to the printf context buffer.
 * @param ctx The printf context.
 * @param c The character to append.
 */
static void ctx_append_char(printf_ctx_t *ctx, char c) {
    if (ctx->pos >= ctx->size)
        ctx_flush(ctx);
    ctx->buffer[ctx->pos++] = c;
    if (c == '\n')
        ctx_flush(ctx);
}

/**
 * @brief Appends a string to the printf context buffer.
 * @param ctx The printf context.
 * @param s The string to append.
 */
static void ctx_append_string(printf_ctx_t *ctx, const char *s) {
    if (!s)
        s = "(null)";
    while (*s)
        ctx_append_char(ctx, *s++);
}

/**
 * @brief Appends an unsigned integer to the printf context buffer.
 * @param ctx The printf context.
 * @param n The unsigned integer to append.
 * @param base The base for number representation.
 */
static void ctx_print_unsigned(printf_ctx_t *ctx, unsigned int n, unsigned int base) {
    char buf[33];
    const char *digits = "0123456789abcdef";
    int i = 0;
    if (base < 2 || base > 16)
        return;
    if (n == 0) {
        ctx_append_char(ctx, '0');
        return;
    }
    while (n > 0) {
        buf[i++] = digits[n % base];
        n /= base;
    }
    while (i-- > 0)
        ctx_append_char(ctx, buf[i]);
}

/**
 * @brief Prints a formatted string to the host environment.
 * @param fmt The format string.
 * @param ... Variable arguments to be formatted.
 * @note This function buffers output and sends it to the host via `__lea_log2`.
 *       It is intended for general logging.
 */
void lea_printf(const char *fmt, ...) {
    char local_buf[128];
    printf_ctx_t ctx = {.buffer = local_buf, .size = sizeof(local_buf), .pos = 0};
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            // --- Parse Length Modifiers ---
            int len_mod = 0; // 0=int, 1=h, 2=hh
            if (*fmt == 'h') {
                len_mod = 1;
                fmt++;
                if (*fmt == 'h') {
                    len_mod = 2;
                    fmt++;
                }
            }

            // --- Handle Specifiers ---
            if (*fmt == '*') {
                fmt++;
                size_t len = va_arg(args, size_t);
                switch (*fmt) {
                case 'x':
                    ctx_append_hex_blob(&ctx, va_arg(args, const unsigned char *), len);
                    break;
                case 'b':
                    ctx_append_binary_blob(&ctx, va_arg(args, const unsigned char *), len);
                    break;
                case 's':
                    ctx_append_sized_string(&ctx, va_arg(args, const char *), len);
                    break;
                }
            } else {
                switch (*fmt) {
                case 'd':
                case 'i': {
                    long long val;
                    if (len_mod == 2)
                        val = (signed char)va_arg(args, int);
                    else if (len_mod == 1)
                        val = (short)va_arg(args, int);
                    else
                        val = va_arg(args, int);

                    if (val < 0) {
                        ctx_append_char(&ctx, '-');
                        ctx_print_unsigned(&ctx, (unsigned int)-val, 10);
                    } else {
                        ctx_print_unsigned(&ctx, (unsigned int)val, 10);
                    }
                    break;
                }
                case 'u':
                case 'x':
                case 'b': {
                    unsigned int uval;
                    if (len_mod == 2)
                        uval = (unsigned char)va_arg(args, unsigned int);
                    else if (len_mod == 1)
                        uval = (unsigned short)va_arg(args, unsigned int);
                    else
                        uval = va_arg(args, unsigned int);

                    unsigned int base = (*fmt == 'x') ? 16 : ((*fmt == 'b') ? 2 : 10);
                    ctx_print_unsigned(&ctx, uval, base);
                    break;
                }
                case 's':
                    ctx_append_string(&ctx, va_arg(args, const char *));
                    break;
                case 'c':
                    ctx_append_char(&ctx, (char)va_arg(args, int));
                    break;
                case '%':
                    ctx_append_char(&ctx, '%');
                    break;
                default:
                    ctx_append_char(&ctx, '%');
                    ctx_append_char(&ctx, *fmt);
                    break;
                }
            }
        } else {
            ctx_append_char(&ctx, *fmt);
        }
        fmt++;
    }
    va_end(args);
    ctx_flush(&ctx);
}

/**
 * @brief Appends a blob of data as a hex string to the snprintf buffer.
 * @param p_curr Pointer to the current position in the buffer.
 * @param end Pointer to the end of the buffer.
 * @param data The data to append.
 * @param len The length of the data.
 */
static void snprintf_append_hex_blob(char **p_curr, char *end, const unsigned char *data,
                                     size_t len) {
    const char *hex_digits = "0123456789abcdef";
    for (size_t i = 0; i < len; ++i) {
        snprintf_append_char(p_curr, end, hex_digits[data[i] >> 4]);
        snprintf_append_char(p_curr, end, hex_digits[data[i] & 0x0F]);
    }
}
/**
 * @brief Appends a blob of data as a hex string to the printf context buffer.
 * @param ctx The printf context.
 * @param data The data to append.
 * @param len The length of the data.
 */
static void ctx_append_hex_blob(printf_ctx_t *ctx, const unsigned char *data, size_t len) {
    const char *hex_digits = "0123456789abcdef";
    for (size_t i = 0; i < len; ++i) {
        ctx_append_char(ctx, hex_digits[data[i] >> 4]);
        ctx_append_char(ctx, hex_digits[data[i] & 0x0F]);
    }
}
/**
 * @brief Appends a blob of data as a binary string to the snprintf buffer.
 * @param p_curr Pointer to the current position in the buffer.
 * @param end Pointer to the end of the buffer.
 * @param data The data to append.
 * @param len The length of the data.
 */
static void snprintf_append_binary_blob(char **p_curr, char *end, const unsigned char *data,
                                        size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (i > 0)
            snprintf_append_char(p_curr, end, ' '); // Add space between bytes for readability
        for (int j = 7; j >= 0; --j) {
            snprintf_append_char(p_curr, end, (data[i] & (1 << j)) ? '1' : '0');
        }
    }
}
/**
 * @brief Appends a blob of data as a binary string to the printf context buffer.
 * @param ctx The printf context.
 * @param data The data to append..
 * @param len The length of the data.
 */
static void ctx_append_binary_blob(printf_ctx_t *ctx, const unsigned char *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (i > 0)
            ctx_append_char(ctx, ' '); // Add space between bytes for readability
        for (int j = 7; j >= 0; --j) {
            ctx_append_char(ctx, (data[i] & (1 << j)) ? '1' : '0');
        }
    }
}
/**
 * @brief Appends a sized string to the snprintf buffer.
 * @param p_curr Pointer to the current position in the buffer.
 * @param end Pointer to the end of the buffer.
 * @param s The string to append.
 * @param len The length of the string.
 */
static void snprintf_append_sized_string(char **p_curr, char *end, const char *s, size_t len) {
    for (size_t i = 0; i < len; ++i)
        snprintf_append_char(p_curr, end, s[i]);
}
/**
 * @brief Appends a sized string to the printf context buffer.
 * @param ctx The printf context.
 * @param s The string to append.
 * @param len The length of the string.
 */
static void ctx_append_sized_string(printf_ctx_t *ctx, const char *s, size_t len) {
    for (size_t i = 0; i < len; ++i)
        ctx_append_char(ctx, s[i]);
}
