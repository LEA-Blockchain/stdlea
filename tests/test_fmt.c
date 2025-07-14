#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlea.h>
#include <string.h>

// A simple testing framework
static int test_count = 0;
static int pass_count = 0;

#define TEST_SNPRINTF(buffer_size, expected_ret, expected_str, fmt, ...)                           \
    do {                                                                                           \
        test_count++;                                                                              \
        char buf[buffer_size];                                                                     \
        int ret = snprintf(buf, sizeof(buf), fmt __VA_OPT__(, ) __VA_ARGS__);                      \
        if (ret == expected_ret && strcmp(buf, expected_str) == 0) {                               \
            pass_count++;                                                                          \
            printf("  [PASS] snprintf(\"%s\", ...)\n", fmt);                                       \
        } else {                                                                                   \
            printf("  [FAIL] snprintf(\"%s\", ...)\n", fmt);                                       \
            printf("         Expected: \"%s\" (ret %d)\n", expected_str, expected_ret);            \
            printf("         Got:      \"%s\" (ret %d)\n", buf, ret);                              \
        }                                                                                          \
    } while (0)

LEA_EXPORT(run_test) int run_test(void) {
    printf("Starting stdio format test...\n\n");

    // --- snprintf tests ---
    printf("--- Testing snprintf ---\n");

    // Basic integer formats
    TEST_SNPRINTF(100, 5, "12345", "%d", 12345);
    TEST_SNPRINTF(100, 6, "-54321", "%i", -54321);
    TEST_SNPRINTF(100, 5, "65535", "%u", 65535);
    TEST_SNPRINTF(100, 4, "ffff", "%x", 0xFFFF);
    TEST_SNPRINTF(100, 8, "10101010", "%b", 0xAA);

    // 64-bit integer formats
    long long s64_val = -9223372036854775807LL;
    unsigned long long u64_val = 18446744073709551615ULL;
    TEST_SNPRINTF(100, 20, "-9223372036854775807", "%lld", s64_val);
    TEST_SNPRINTF(100, 20, "18446744073709551615", "%llu", u64_val);
    TEST_SNPRINTF(100, 16, "ffffffffffffffff", "%llx", u64_val);
    TEST_SNPRINTF(100, 1, "0", "%lld", 0LL);

    // String, char, percent
    TEST_SNPRINTF(100, 11, "hello world", "%s", "hello world");
    TEST_SNPRINTF(100, 6, "(null)", "%s", NULL);
    TEST_SNPRINTF(100, 1, "Z", "%c", 'Z');
    TEST_SNPRINTF(100, 1, "%", "%%", 0);

    // Mixed formats
    TEST_SNPRINTF(100, 29, "User: Alice, ID: 123, Hex: 7b", "User: %s, ID: %d, Hex: %x", "Alice",
                  123, 123);
    TEST_SNPRINTF(100, 38, "Big num: 18446744073709551615, Char: X", "Big num: %llu, Char: %c",
                  u64_val, 'X');

    // Buffer truncation
    TEST_SNPRINTF(10, 12, "123456789", "%s", "123456789012"); // ret > size-1
    TEST_SNPRINTF(5, 4, "1234", "%d", 1234);                  // ret == size-1
    TEST_SNPRINTF(4, 4, "123", "%d", 1234);                   // ret > size-1

    // Arbitrary-length specifiers
    const unsigned char blob[] = {0xDE, 0xAD, 0xBE, 0xEF};
    TEST_SNPRINTF(100, 8, "deadbeef", "%*x", (size_t)4, blob);
    TEST_SNPRINTF(100, 35, "11011110 10101101 10111110 11101111", "%*b", (size_t)4, blob);
    TEST_SNPRINTF(100, 6, "lea-vm", "%*s", (size_t)6, "lea-vm-rocks");

    // --- printf tests (visual inspection) ---
    printf("\n--- Testing printf (visual inspection) ---\n");
    printf("  Signed 32-bit: %d\n", -12345);
    printf("  Unsigned 32-bit: %u\n", 12345);
    printf("  Hex 32-bit: %x\n", 0xABCDEF);
    printf("  Binary 32-bit: %b\n", 0xCC);
    printf("  Signed 64-bit: %lld\n", -123456789012345LL);
    printf("  Unsigned 64-bit: %llu\n", 123456789012345ULL);
    printf("  Hex 64-bit: %llx\n", 0x123456789ABCDEF0LL);
    printf("  String: %s\n", "This is a test string.");
    printf("  Char: %c\n", 'A');
    printf("  Percent: %%\n");
    printf("  Blob Hex: %*x\n", (size_t)4, blob);
    printf("  Blob Bin: %*b\n", (size_t)4, blob);
    printf("  Sized Str: %*s\n", (size_t)8, "lea-vm-rocks");
    printf("  Complex: Val=%d, Str=%s, U64=%llu\n", -42, "test", 9876543210987654321ULL);

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
