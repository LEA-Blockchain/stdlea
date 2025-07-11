#include "stdlea.h"
#include "string.h"

LEA_IMPORT(env, __lea_log)
void __lea_log(const char *);

// Global log buffer
char lea_log_buffer[LEA_LOG_BUFFER_SIZE] = {0};

/**
 * @brief Logs a message by copying it to a global buffer and passing it to the host.
 * @param message The null-terminated string to log.
 * @note This function uses a global buffer `lea_log_buffer`. If the message
 *       is longer than the buffer, it will be truncated. The buffer is cleared
 *       after the log message is passed to the host. A NULL message is
 *       treated as an empty string.
 */
void lea_log(const char *message) {
    // Handle NULL message by treating it as an empty string
    if (!message) {
        lea_log_buffer[0] = '\0';
        return;
    }

    size_t i = 0;

    // Copy characters while the source has characters and the buffer has space.
    // We stop at LEA_LOG_BUFFER_SIZE - 1 to guarantee space for the null terminator.
    while (message[i] != '\0' && i < (LEA_LOG_BUFFER_SIZE - 1)) {
        lea_log_buffer[i] = message[i];
        i++;
    }

    // Add the null terminator at the end.
    lea_log_buffer[i] = '\0';

    __lea_log(lea_log_buffer);
    memset(lea_log_buffer, 0, LEA_LOG_BUFFER_SIZE); // Clear the buffer after logging
}
