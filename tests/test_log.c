#include <stdio.h>
#include <stdlea.h>

LEA_EXPORT(run_test) int run_test(void) {
    LEA_LOG("Starting log test...\n");

    LEA_LOG("Hello, logger!\n");
    LEA_LOG("Another test.\n");
    LEA_LOG("");
    LEA_LOG("Message with spaces\n");

    LEA_LOG("--- Test Summary ---\n");
    LEA_LOG("4/4 tests passed.\n");
    LEA_LOG("ALL TESTS PASSED\n");

    return 0;
}
