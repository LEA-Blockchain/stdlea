#include "stdio.h"
#include "stdlea.h"
#include "stdlib.h"
#include "string.h"

// A simple testing framework
static int test_count = 0;
static int pass_count = 0;

#define ASSERT_TRUE(condition, msg)                                                                \
    do {                                                                                           \
        test_count++;                                                                              \
        if (condition) {                                                                           \
            pass_count++;                                                                          \
            printf("  [PASS] %s\n", msg);                                                          \
        } else {                                                                                   \
            printf("  [FAIL] %s at line %d\n", msg, __LINE__);                                     \
        }                                                                                          \
    } while (0)

#define ASSERT_EQ(val1, val2, msg) ASSERT_TRUE((val1) == (val2), msg)

void test_strlen(void) {
    printf("\n--- Testing strlen ---\n");
    ASSERT_EQ(strlen(""), 0, "strlen of empty string");
    ASSERT_EQ(strlen("hello"), 5, "strlen of 'hello'");
    ASSERT_EQ(strlen("a\0b"), 1, "strlen with null terminator");
}

void test_strcmp(void) {
    printf("\n--- Testing strcmp ---\n");
    ASSERT_TRUE(strcmp("a", "a") == 0, "strcmp equal");
    ASSERT_TRUE(strcmp("a", "b") < 0, "strcmp less than");
    ASSERT_TRUE(strcmp("b", "a") > 0, "strcmp greater than");
    ASSERT_TRUE(strcmp("abc", "abd") < 0, "strcmp prefix less than");
    ASSERT_TRUE(strcmp("abc", "abb") > 0, "strcmp prefix greater than");
    ASSERT_TRUE(strcmp("short", "longer") > 0, "strcmp different lengths");
    ASSERT_TRUE(strcmp("", "") == 0, "strcmp empty strings");
}

void test_strncmp(void) {
    printf("\n--- Testing strncmp ---\n");
    ASSERT_TRUE(strncmp("abc", "abd", 2) == 0, "strncmp equal up to n");
    ASSERT_TRUE(strncmp("abc", "abd", 3) < 0, "strncmp unequal at n");
    ASSERT_TRUE(strncmp("a", "b", 0) == 0, "strncmp with n=0");
    ASSERT_TRUE(strncmp("abc", "abc", 10) == 0, "strncmp with n > len");
}

void test_memset(void) {
    printf("\n--- Testing memset ---\n");
    char buf[10];
    memset(buf, 'A', 5);
    buf[5] = '\0';
    ASSERT_TRUE(strcmp(buf, "AAAAA") == 0, "memset basic");
    memset(buf, 0, 10);
    for (int i = 0; i < 10; i++) {
        if (buf[i] != 0) {
            ASSERT_TRUE(0, "memset to zero");
            return;
        }
    }
    ASSERT_TRUE(1, "memset to zero");
}

void test_memcpy(void) {
    printf("\n--- Testing memcpy ---\n");
    char src[] = "hello";
    char dst[6];
    memcpy(dst, src, 6); // include null terminator
    ASSERT_TRUE(strcmp(dst, "hello") == 0, "memcpy basic");
    ASSERT_TRUE(dst[5] == '\0', "memcpy null terminator");
}

void test_memmove(void) {
    printf("\n--- Testing memmove ---\n");
    char buf[20] = "abcdefghij";
    // Overlap, dest > src
    memmove(buf + 2, buf, 5); // "ababcdehij"
    ASSERT_TRUE(strcmp(buf, "ababcdehij") == 0, "memmove overlapping dest > src");

    strcpy(buf, "abcdefghij");
    // Overlap, dest < src
    memmove(buf, buf + 2, 5); // "cdefgfghij"
    ASSERT_TRUE(strcmp(buf, "cdefgfghij") == 0, "memmove overlapping dest < src");
}

void test_memcmp(void) {
    printf("\n--- Testing memcmp ---\n");
    char s1[] = "abcde";
    char s2[] = "abcdf";
    ASSERT_TRUE(memcmp(s1, s2, 4) == 0, "memcmp equal prefix");
    ASSERT_TRUE(memcmp(s1, s2, 5) < 0, "memcmp less than");
    ASSERT_TRUE(memcmp(s2, s1, 5) > 0, "memcmp greater than");
    ASSERT_TRUE(memcmp("a", "a", 1) == 0, "memcmp single char equal");
}

LEA_EXPORT(run_test) int run_test(void) {
    printf("Starting string functions test...\n");

    test_strlen();
    test_strcmp();
    test_strncmp();
    test_memset();
    test_memcpy();
    test_memmove();
    test_memcmp();

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
