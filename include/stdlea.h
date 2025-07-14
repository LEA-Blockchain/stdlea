#ifndef STDLEA_H
#define STDLEA_H

#include "stddef.h"
#include <stdint.h>

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
#define LEA_IMPORT(PROGRAM_ID, FUNC_NAME)                                                          \
    __attribute__((import_module(#PROGRAM_ID), import_name(#FUNC_NAME))) extern

/**
 * @def IF_LEA_EXPORT(FUNC_NAME)
 * @brief A conditional version of LEA_EXPORT, intended for future use.
 * @param FUNC_NAME The name of the function.
 */
#define IF_LEA_EXPORT(FUNC_NAME) LEA_EXPORT(FUNC_NAME)

/**
 * @def LEA_ABORT()
 * @brief Aborts the program execution immediately.
 * @note This macro signals an unrecoverable error to the host environment,
 *       providing the current line number, and then halts execution.
 */
LEA_IMPORT(env, __lea_abort) void __lea_abort(int);
#define LEA_ABORT()                                                                                \
    do {                                                                                           \
        __lea_abort(__LINE__);                                                                     \
        __builtin_trap();                                                                          \
    } while (0)

#ifdef ENABLE_LEA_LOG
/**
 * @brief Logs a message to the host environment.
 * @param message The null-terminated string to log.
 * @note This function is only available when `ENABLE_LEA_LOG` is defined.
 */
void lea_log(const char *message);

/**
 * @def LEA_LOG(MSG)
 * @brief A macro to log a message.
 * @param MSG The message string to log.
 * @note This macro compiles to nothing if `ENABLE_LEA_LOG` is not defined.
 */
#define LEA_LOG(MSG) lea_log(MSG)
#else
#define LEA_LOG(MSG) ((void)0)
#endif // ENABLE_LEA_LOG

#ifndef DISABLE_BUMP_ALLOCATOR
/**
 * @brief Resets the heap allocator.
 * @note Zeros out the heap and resets the allocation pointer.
 */
void allocator_reset();

/** @name Heap and Buffer Configuration */
/** @{ */
/** @def LEA_HEAP_SIZE
 *  @brief The total size of the static heap in bytes.
 */
#define LEA_HEAP_SIZE 1048576 // 1 MiB heap size

#endif // DISABLE_BUMP_ALLOCATOR

#endif // STDLEA_H
