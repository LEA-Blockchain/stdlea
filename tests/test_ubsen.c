#include "stdlea.h"
#include <limits.h>

LEA_EXPORT(overflow) int overflow(void) {
    volatile int x = INT_MAX;
    x++; // This will trigger an integer overflow.
    return 0;
}

LEA_EXPORT(underflow) int underflow(void) {
    volatile int x = INT_MIN;
    x--; // This will trigger an integer overflow.
    return 0;
}
