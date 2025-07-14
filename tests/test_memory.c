#include "stdio.h"
#include "stdlea.h"
#include "stdlib.h"
#include "string.h"

// A simple testing framework
static int test_count = 0;
static int pass_count = 0;

#define ASSERT(condition)                                                                          \
    do {                                                                                           \
        test_count++;                                                                              \
        if (condition) {                                                                           \
            pass_count++;                                                                          \
            printf("  [PASS] %s\n", #condition);                                                   \
        } else {                                                                                   \
            printf("  [FAIL] %s at line %d\n", #condition, __LINE__);                              \
        }                                                                                          \
    } while (0)

LEA_EXPORT(run_test) int run_test(void) {
    printf("Starting memory allocator test...\n\n");

    // --- Malloc tests ---
    printf("--- Testing malloc ---\n");
    void *p1 = malloc(10);
    ASSERT(p1 != NULL);

    void *p2 = malloc(20);
    ASSERT(p2 != NULL);
    ASSERT(p2 > p1); // Bump allocator should return increasing addresses
    ASSERT((size_t)p2 - (size_t)p1 == 10);

    // --- Allocator Reset Test ---
    printf("\n--- Testing allocator_reset ---\n");
    allocator_reset();
    void *p3 = malloc(10);
    ASSERT(p3 != NULL);
    ASSERT(p3 == p1); // After reset, allocation should start from the beginning

    // --- Out of memory test ---
    printf("\n--- Testing out-of-memory ---\n");
    allocator_reset();
    void *p4 = malloc(LEA_HEAP_SIZE - 1);
    ASSERT(p4 != NULL);
    printf("  Allocation of LEA_HEAP_SIZE - 1 should succeed.\n");
    printf("  The next allocation should abort...\n");
    // This next line should trigger the abort. The JS runner will report "ABORT at line...".
    malloc(2);
    // Code below here should not be reached.
    printf("  [FAIL] Program did not abort on out-of-memory.\n");

    // This part of the test will not be reached if the abort works correctly.
    // It's here to show failure if the abort doesn't happen.
    printf("\n--- Test Summary ---\n");
    printf("%d/%d tests passed.\n", pass_count, test_count);

    if (pass_count == test_count) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED\n");
        return 1;
    }
}
