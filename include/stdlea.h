#ifndef STDLEA_H
#define STDLEA_H

#include "c_feature_check.h"
#include "wasm_feature_check.h"

// --- Standard Integer Types ---

/** @file stdlea.h
 *  @brief Standard library for Lea programs.
 *  This file provides standard type definitions, macros, and function
 *  declarations for use in Lea programs. It ensures a consistent
 *  environment, whether compiling for 32-bit or 64-bit Wasm targets.
 */

// --- Standard Integer Types ---

/** @name Fixed-Width Integer Types */
/** @{ */
typedef unsigned char uint8_t; /**< Unsigned 8-bit integer. */
typedef unsigned short uint16_t; /**< Unsigned 16-bit integer. */
typedef unsigned int uint32_t; /**< Unsigned 32-bit integer. */
typedef unsigned long long uint64_t; /**< Unsigned 64-bit integer. */

typedef signed char int8_t; /**< Signed 8-bit integer. */
typedef signed short int16_t; /**< Signed 16-bit integer. */
typedef signed int int32_t; /**< Signed 32-bit integer. */
typedef signed long long int64_t; /**< Signed 64-bit integer. */
/** @} */

/** @name Integer Limits */
/** @{ */
#ifndef UINT8_MAX
#define UINT8_MAX (255U)
#endif
#ifndef INT8_MAX
#define INT8_MAX (127)
#endif
#ifndef INT8_MIN
#define INT8_MIN (-128)
#endif

#ifndef UINT16_MAX
#define UINT16_MAX (65535U)
#endif
#ifndef INT16_MAX
#define INT16_MAX (32767)
#endif
#ifndef INT16_MIN
#define INT16_MIN (-32768)
#endif

#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif
#ifndef INT32_MAX
#define INT32_MAX (2147483647)
#endif
#ifndef INT32_MIN
#define INT32_MIN (-2147483647 - 1)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX (18446744073709551615ULL)
#endif
#ifndef INT64_MAX
#define INT64_MAX (9223372036854775807LL)
#endif
#ifndef INT64_MIN
#define INT64_MIN (-9223372036854775807LL - 1)
#endif
/** @} */

/** @name Boolean Type and Values */
/** @{ */
#ifndef bool
typedef _Bool bool; /**< Boolean type. */
#endif

#ifndef true
/** @def true Represents the boolean value true (1). */
#define true ((bool)1)
#endif
#ifndef false
/** @def false Represents the boolean value false (0). */
#define false ((bool)0)
#endif
/** @} */

/** @name Pointer and Size Types */
/** @{ */
#if defined(__wasm64__) || (defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 8)
/* --- 64-bit Wasm --- */
/** @typedef uintptr_t Unsigned integer type capable of holding a pointer for 64-bit targets. */
typedef uint64_t uintptr_t;
/** @typedef intptr_t Signed integer type capable of holding a pointer for 64-bit targets. */
typedef int64_t intptr_t;
/** @typedef size_t Type for representing sizes of objects for 64-bit targets. */
typedef uint64_t size_t;

#ifndef UINTPTR_MAX
#define UINTPTR_MAX UINT64_MAX
#endif
#ifndef INTPTR_MAX
#define INTPTR_MAX INT64_MAX
#endif
#ifndef INTPTR_MIN
#define INTPTR_MIN INT64_MIN
#endif
#ifndef SIZE_MAX
#define SIZE_MAX UINT64_MAX
#endif

#elif defined(__wasm32__) || (defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 4)
/* --- 32-bit Wasm --- */
/** @typedef uintptr_t Unsigned integer type capable of holding a pointer for 32-bit targets. */
typedef uint32_t uintptr_t;
/** @typedef intptr_t Signed integer type capable of holding a pointer for 32-bit targets. */
typedef int32_t intptr_t;
/** @typedef size_t Type for representing sizes of objects for 32-bit targets. */
typedef uint32_t size_t;

#ifndef UINTPTR_MAX
#define UINTPTR_MAX UINT32_MAX
#endif
#ifndef INTPTR_MAX
#define INTPTR_MAX INT32_MAX
#endif
#ifndef INTPTR_MIN
#define INTPTR_MIN INT32_MIN
#endif
#ifndef SIZE_MAX
#define SIZE_MAX UINT32_MAX
#endif

#else
#error "Wasm target detected, but cannot determine pointer size (require wasm32 or wasm64)."
#endif

#ifndef NULL
/** @def NULL Represents a null pointer. */
#define NULL ((void *)0)
#endif
/** @} */

/** @name Function Annotations */
/** @{ */
/**
 * @def LEA_EXPORT(FUNC_NAME)
 * @brief Marks a function for export from the Wasm module.
 * @param FUNC_NAME The name of the function to export.
 */
#define LEA_EXPORT(FUNC_NAME) __attribute__((export_name(#FUNC_NAME), visibility("default")))

/**
 * @def LEA_IMPORT(PROGRAM_ID, FUNC_NAME)
 * @brief Marks a function for import into the Wasm module.
 * @param PROGRAM_ID The module from which to import.
 * @param FUNC_NAME The name of the function to import.
 */
#define LEA_IMPORT(PROGRAM_ID, FUNC_NAME) __attribute__((import_module(#PROGRAM_ID), import_name(#FUNC_NAME))) extern

/**
 * @def IF_LEA_EXPORT(FUNC_NAME)
 * @brief A conditional version of LEA_EXPORT, intended for future use.
 * @param FUNC_NAME The name of the function.
 */
#define IF_LEA_EXPORT(FUNC_NAME) LEA_EXPORT(FUNC_NAME)
/** @} */

/** @name Logging and Aborting */
/** @{ */
#ifdef ENABLE_LEA_LOG
/**
 * @brief Logs a message to the host environment.
 * @param message The null-terminated string to log.
 * @note This function is only available when `ENABLE_LEA_LOG` is defined.
 *       It uses a global buffer and may truncate long messages.
 */
void lea_log(const char *message);

/**
 * @def lea_abort(ERROR_MESSAGE)
 * @brief Aborts execution after logging a message.
 * @param ERROR_MESSAGE The error message to log before aborting.
 * @note This macro calls `lea_log` and then executes a trap instruction.
 */
#define lea_abort(ERROR_MESSAGE)          \
    do                                    \
    {                                     \
        lea_log("ABORT: " ERROR_MESSAGE); \
        __builtin_trap();                 \
    } while (0)

/**
 * @def LEA_LOG(MSG)
 * @brief Logs a message.
 * @param MSG The message to log.
 * @note This is a convenience macro for `lea_log`.
 */
#define LEA_LOG(MSG) lea_log(MSG)
#else
/**
 * @def LEA_LOG(MSG)
 * @brief A no-op logging macro.
 * @param MSG The message, which is ignored.
 * @note This macro does nothing when `ENABLE_LEA_LOG` is not defined.
 */
#define LEA_LOG(MSG) ((void)0)

/**
 * @def lea_abort(ERROR_MESSAGE)
 * @brief Aborts execution immediately.
 * @param ERROR_MESSAGE The message, which is ignored.
 * @note This macro executes a trap instruction without logging.
 */
#define lea_abort(ERROR_MESSAGE) __builtin_trap()
#endif
/** @} */

/** @name Memory Functions */
/** @{ */
void *memset(void *dest, int val, size_t len);
void *memcpy(void *dest, const void *src, size_t len);
void *memmove(void *dest, const void *src, size_t len);

/**
 * @brief Allocates memory from the heap.
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory.
 * @warning This is a simple bump allocator. No `free` is available.
 */
void *malloc(size_t size);

/**
 * @brief Resets the heap allocator.
 * @note Zeros out the heap and resets the allocation pointer.
 */
void allocator_reset();
/** @} */

/** @name Heap and Buffer Configuration */
/** @{ */
/** @def LEA_HEAP_SIZE
 *  @brief The total size of the static heap in bytes.
 */
#define LEA_HEAP_SIZE 65536 // 64 KiB heap size

/** @def LEA_LOG_BUFFER_SIZE
 *  @brief The size of the buffer used for logging messages.
 */
#define LEA_LOG_BUFFER_SIZE 512
/** @} */

#ifndef SIZE_MAX
/** @def SIZE_MAX
 *  @brief The maximum value for a `size_t` object.
 */
#define SIZE_MAX ((size_t)-1)
#endif

#endif // STDLEA_H
