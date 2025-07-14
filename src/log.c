#include "stddef.h"
#include "stdlea.h"

#ifdef ENABLE_LEA_LOG
LEA_IMPORT(env, __lea_log) void __lea_log(const char *, size_t);
void lea_log(const char *message) {
    size_t len = 0;
    while (message[len])
        len++;
    __lea_log(message, len);
}
#endif
