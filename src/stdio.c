#include "stdio.h"
#include "stddef.h"
#include "stdlea.h"
#include <stdarg.h>

#ifdef ENABLE_LEA_FMT
LEA_IMPORT(env, __lea_log) void __lea_log(const char *, size_t);

// Forward declarations for helpers
typedef struct printf_ctx_s printf_ctx_t;
static void ctx_append_hex_blob(printf_ctx_t *ctx, const unsigned char *data, size_t len);
static void ctx_append_binary_blob(printf_ctx_t *ctx, const unsigned char *data, size_t len);
static void ctx_append_sized_string(printf_ctx_t *ctx, const char *s, size_t len);
static void ctx_print_unsigned_long_long(printf_ctx_t *ctx, unsigned long long n,
                                         unsigned int base);

// A helper struct to manage the state of the snprintf operation
typedef struct {
    char *buf;       // The start of the output buffer
    char *p;         // The current write position in the buffer
    const char *end; // The end of the writable part of the buffer (buf + size - 1)
    int total;       // The total number of characters that would have been written
} vsnprintf_state_t;

/**
 * @brief Appends a single character to the vsnprintf buffer.
 * @param state The current state of the vsnprintf operation.
 * @param c The character to append.
 * @note This function updates the buffer pointer and total count.
 */
static void stateful_append_char(vsnprintf_state_t *state, char c) {
    if (state->p < state->end) {
        *state->p = c;
        state->p++;
    }
    state->total++;
}

/**
 * @brief Appends a null-terminated string to the vsnprintf buffer.
 * @param state The current state of the vsnprintf operation.
 * @param s The string to append. A NULL string is printed as "(null)".
 */
static void stateful_append_string(vsnprintf_state_t *state, const char *s) {
    if (!s)
        s = "(null)";
    while (*s) {
        stateful_append_char(state, *s++);
    }
}

/**
 * @brief Prints an unsigned long long to the vsnprintf buffer in a given base.
 * @param state The current state of the vsnprintf operation.
 * @param n The number to print.
 * @param base The numeric base (e.g., 10 for decimal, 16 for hex).
 */
static void stateful_print_ull(vsnprintf_state_t *state, unsigned long long n, unsigned int base) {
    char buf[65];
    const char *digits = "0123456789abcdef";
    int i = 0;
    if (base < 2 || base > 16)
        return;

    if (n == 0) {
        stateful_append_char(state, '0');
        return;
    }

    while (n > 0) {
        buf[i++] = digits[n % base];
        n /= base;
    }

    while (i-- > 0) {
        stateful_append_char(state, buf[i]);
    }
}

int vsnprintf(char *buffer, size_t size, const char *fmt, va_list args) {
    vsnprintf_state_t state = {
        .buf = buffer, .p = buffer, .end = (size > 0) ? buffer + size - 1 : buffer, .total = 0};

    // If size is 0, we can't even write a null terminator, but we still calculate the total length.
    if (size == 0) {
        state.end = NULL;
    }

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            int len_mod = 0; // 0=int, 1=h, 2=hh, 3=ll
            if (*fmt == 'h') {
                len_mod = 1;
                fmt++;
                if (*fmt == 'h') {
                    len_mod = 2;
                    fmt++;
                }
            } else if (*fmt == 'l' && *(fmt + 1) == 'l') {
                len_mod = 3;
                fmt += 2;
            }

            if (*fmt == '*') {
                fmt++;
                size_t len = va_arg(args, size_t);
                const unsigned char *blob;
                const char *str;
                const char *hex_digits = "0123456789abcdef";
                switch (*fmt) {
                case 'x':
                    blob = va_arg(args, const unsigned char *);
                    for (size_t i = 0; i < len; ++i) {
                        stateful_append_char(&state, hex_digits[blob[i] >> 4]);
                        stateful_append_char(&state, hex_digits[blob[i] & 0x0F]);
                    }
                    break;
                case 'b':
                    blob = va_arg(args, const unsigned char *);
                    for (size_t i = 0; i < len; ++i) {
                        if (i > 0)
                            stateful_append_char(&state, ' ');
                        for (int j = 7; j >= 0; --j) {
                            stateful_append_char(&state, (blob[i] & (1 << j)) ? '1' : '0');
                        }
                    }
                    break;
                case 's':
                    str = va_arg(args, const char *);
                    for (size_t i = 0; i < len; ++i) {
                        stateful_append_char(&state, str[i]);
                    }
                    break;
                }
            } else {
                switch (*fmt) {
                case 'd':
                case 'i': {
                    long long val;
                    if (len_mod == 3)
                        val = va_arg(args, long long);
                    else if (len_mod == 2)
                        val = (signed char)va_arg(args, int);
                    else if (len_mod == 1)
                        val = (short)va_arg(args, int);
                    else
                        val = va_arg(args, int);

                    if (val < 0) {
                        stateful_append_char(&state, '-');
                        stateful_print_ull(&state, (unsigned long long)-val, 10);
                    } else {
                        stateful_print_ull(&state, (unsigned long long)val, 10);
                    }
                    break;
                }
                case 'u':
                case 'x':
                case 'b': {
                    unsigned long long uval;
                    if (len_mod == 3)
                        uval = va_arg(args, unsigned long long);
                    else if (len_mod == 2)
                        uval = (unsigned char)va_arg(args, unsigned int);
                    else if (len_mod == 1)
                        uval = (unsigned short)va_arg(args, unsigned int);
                    else
                        uval = va_arg(args, unsigned int);

                    unsigned int base = (*fmt == 'x') ? 16 : ((*fmt == 'b') ? 2 : 10);
                    stateful_print_ull(&state, uval, base);
                    break;
                }
                case 's':
                    stateful_append_string(&state, va_arg(args, const char *));
                    break;
                case 'c':
                    stateful_append_char(&state, (char)va_arg(args, int));
                    break;
                case '%':
                    stateful_append_char(&state, '%');
                    break;
                default:
                    stateful_append_char(&state, '%');
                    stateful_append_char(&state, *fmt);
                    break;
                }
            }
        } else {
            stateful_append_char(&state, *fmt);
        }
        fmt++;
    }

    if (size > 0) {
        *state.p = '\0';
    }

    return state.total;
}

int snprintf(char *buffer, size_t size, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = vsnprintf(buffer, size, fmt, args);
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
        __lea_log(ctx->buffer, ctx->pos);
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
 * @brief Appends an unsigned long long to the printf context buffer.
 * @param ctx The printf context.
 * @param n The unsigned long long to append.
 * @param base The base for number representation.
 */
static void ctx_print_unsigned_long_long(printf_ctx_t *ctx, unsigned long long n,
                                         unsigned int base) {
    char buf[65]; // Sufficient for 64-bit binary representation
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

void printf(const char *fmt, ...) {

    char local_buf[128];
    printf_ctx_t ctx = {.buffer = local_buf, .size = sizeof(local_buf), .pos = 0};
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            // --- Parse Length Modifiers ---
            int len_mod = 0; // 0=int, 1=h, 2=hh, 3=ll
            if (*fmt == 'h') {
                len_mod = 1;
                fmt++;
                if (*fmt == 'h') {
                    len_mod = 2;
                    fmt++;
                }
            } else if (*fmt == 'l' && *(fmt + 1) == 'l') {
                len_mod = 3; // ll
                fmt += 2;
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
                    if (len_mod == 3) { // ll
                        val = va_arg(args, long long);
                    } else if (len_mod == 2) { // hh
                        val = (signed char)va_arg(args, int);
                    } else if (len_mod == 1) { // h
                        val = (short)va_arg(args, int);
                    } else { // int
                        val = va_arg(args, int);
                    }

                    if (val < 0) {
                        ctx_append_char(&ctx, '-');
                        unsigned long long uval = (unsigned long long)-val;
                        if (len_mod == 3) {
                            ctx_print_unsigned_long_long(&ctx, uval, 10);
                        } else {
                            ctx_print_unsigned(&ctx, (unsigned int)uval, 10);
                        }
                    } else {
                        unsigned long long uval = (unsigned long long)val;
                        if (len_mod == 3) {
                            ctx_print_unsigned_long_long(&ctx, uval, 10);
                        } else {
                            ctx_print_unsigned(&ctx, (unsigned int)uval, 10);
                        }
                    }
                    break;
                }
                case 'u':
                case 'x':
                case 'b': {
                    unsigned long long uval;
                    if (len_mod == 3) { // ll
                        uval = va_arg(args, unsigned long long);
                    } else if (len_mod == 2) { // hh
                        uval = (unsigned char)va_arg(args, unsigned int);
                    } else if (len_mod == 1) { // h
                        uval = (unsigned short)va_arg(args, unsigned int);
                    } else { // int
                        uval = va_arg(args, unsigned int);
                    }

                    unsigned int base = (*fmt == 'x') ? 16 : ((*fmt == 'b') ? 2 : 10);
                    if (len_mod == 3) {
                        ctx_print_unsigned_long_long(&ctx, uval, base);
                    } else {
                        ctx_print_unsigned(&ctx, (unsigned int)uval, base);
                    }
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
 * @brief Appends a sized string to the printf context buffer.
 * @param ctx The printf context.
 * @param s The string to append.
 * @param len The length of the string.
 */
static void ctx_append_sized_string(printf_ctx_t *ctx, const char *s, size_t len) {
    for (size_t i = 0; i < len; ++i)
        ctx_append_char(ctx, s[i]);
}

#endif // ENABLE_LEA_FMT
