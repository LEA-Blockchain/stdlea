#ifndef STDDEF_H
#define STDDEF_H

/**
 * @brief Unsigned integer type for sizes.
 * @note This is defined as `unsigned int` for WASM 32-bit compatibility.
 */
typedef unsigned int size_t;

/**
 * @brief Signed integer type for pointer differences.
 * @note This is defined as `int` for WASM 32-bit compatibility.
 */
typedef int ptrdiff_t;

/**
 * @def NULL
 * @brief A macro representing a null pointer constant.
 */
#ifndef NULL
#define NULL ((void *)0)
#endif

/**
 * @def offsetof(type, member)
 * @brief Calculates the byte offset of a member within a structure.
 * @param type The structure type.
 * @param member The member of the structure.
 * @return The offset of the member in bytes from the beginning of the structure.
 */
#define offsetof(type, member) ((size_t) & (((type *)0)->member))

// Static asserts to confirm sizes are correct for 32-bit WASM
_Static_assert(sizeof(size_t) == 4, "size_t must be 4 bytes (WASM 32-bit)");
_Static_assert(sizeof(ptrdiff_t) == 4, "ptrdiff_t must be 4 bytes (WASM 32-bit)");

#endif // STDDEF_H
