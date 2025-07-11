#ifndef STDLEA_H
#define STDLEA_H
#include "feature_check.h"

#include "stddef.h"

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
#define lea_abort(ERROR_MESSAGE)                                                                   \
    do {                                                                                           \
        lea_log("ABORT: " ERROR_MESSAGE);                                                          \
        __builtin_trap();                                                                          \
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

/**
 * @brief Resets the heap allocator.
 * @return void
 * @note Zeros out the heap and resets the allocation pointer.
 */
void allocator_reset();

/** @name Heap and Buffer Configuration */
/** @{ */
/** @def LEA_HEAP_SIZE
 *  @brief The total size of the static heap in bytes.
 */
#define LEA_HEAP_SIZE 1048576 // 1 MiB heap size

/** @def LEA_LOG_BUFFER_SIZE
 *  @brief The size of the buffer used for logging messages.
 */
#define LEA_LOG_BUFFER_SIZE 512

#endif // STDLEA_H
