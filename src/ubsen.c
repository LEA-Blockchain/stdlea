#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

#include "stdlea.h"
#include <stdio.h>

#ifdef ENABLE_LEA_UBSAN

LEA_IMPORT(env, __lea_ubsen)
void __lea_ubsen(const char *name, const char *filename, int line, int column);

#define LEA_ABORT_UBSEN(HANDLER_NAME, FILENAME, LINE, COLUMN)                                      \
    do {                                                                                           \
        __lea_ubsen(HANDLER_NAME, FILENAME, LINE, COLUMN);                                         \
        __builtin_trap();                                                                          \
    } while (0)

// All UBSan Struct Definitions

struct __ubsan_source_location {
    const char *filename;
    int line;
    int column;
};

struct __ubsan_overflow_data {
    struct __ubsan_source_location location;
    const char *type;
};

struct __ubsan_type_mismatch_data {
    struct __ubsan_source_location location;
    const char *type;
    unsigned char alignment;
    unsigned char kind;
};

struct __ubsan_shift_out_of_bounds_data {
    struct __ubsan_source_location location;
    const char *lhs_type;
    const char *rhs_type;
};

struct __ubsan_out_of_bounds_data {
    struct __ubsan_source_location location;
    const char *array_type;
};

struct __ubsan_invalid_value_data {
    struct __ubsan_source_location location;
    const char *type;
};

struct __ubsan_function_type_mismatch_data {
    struct __ubsan_source_location location;
};

struct __ubsan_nonnull_arg_data {
    struct __ubsan_source_location location;
};

struct __ubsan_implicit_conversion_data {
    struct __ubsan_source_location location;
    const char *from_type;
    const char *to_type;
};

struct __ubsan_vla_bound_data {
    struct __ubsan_source_location location;
    const char *type;
};

// Macro for handlers that receive location data.
#define UBSAN_HANDLER_WITH_LOCATION(name, ...)                                                     \
    __attribute__((visibility("default"))) void name(__VA_ARGS__) {                                \
        struct __ubsan_source_location *loc = &data->location;                                     \
        /* Optional: a printf for local debugging. Can be removed to save space. */                \
        printf("[UBSAN] Triggered: %s at %s:%d:%d\n", #name, loc->filename, loc->line,             \
               loc->column);                                                                       \
        /* Call the abort macro with the handler name and location info. */                        \
        LEA_ABORT_UBSEN(#name, loc->filename, loc->line, loc->column);                             \
    }

// Macro for simple handlers without location data.
#define UBSAN_HANDLER_SIMPLE(name, ...)                                                            \
    __attribute__((visibility("default"))) void name(__VA_ARGS__) {                                \
        /* Optional: a printf for local debugging. */                                              \
        printf("[UBSAN] Triggered: %s\n", #name);                                                  \
        /* Call the abort macro with the handler name and NULL/0 for location. */                  \
        LEA_ABORT_UBSEN(#name, NULL, 0, 0);                                                        \
    }

// Handlers WITH Location Data
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_add_overflow, struct __ubsan_overflow_data *data,
                            void *lhs, void *rhs)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_sub_overflow, struct __ubsan_overflow_data *data,
                            void *lhs, void *rhs)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_mul_overflow, struct __ubsan_overflow_data *data,
                            void *lhs, void *rhs)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_divrem_overflow, struct __ubsan_overflow_data *data,
                            void *lhs, void *rhs)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_negate_overflow, struct __ubsan_overflow_data *data,
                            void *val)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_type_mismatch_v1,
                            struct __ubsan_type_mismatch_data *data, void *ptr)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_shift_out_of_bounds,
                            struct __ubsan_shift_out_of_bounds_data *data, void *lhs, void *rhs)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_out_of_bounds, struct __ubsan_out_of_bounds_data *data,
                            void *index)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_load_invalid_value,
                            struct __ubsan_invalid_value_data *data, void *val)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_function_type_mismatch,
                            struct __ubsan_function_type_mismatch_data *data, void *fn)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_nonnull_arg, struct __ubsan_nonnull_arg_data *data)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_implicit_conversion,
                            struct __ubsan_implicit_conversion_data *data, void *from, void *to)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_invalid_enum, struct __ubsan_invalid_value_data *data,
                            void *val)
UBSAN_HANDLER_WITH_LOCATION(__ubsan_handle_vla_bound_not_positive,
                            struct __ubsan_vla_bound_data *data, void *bound)

// Handlers WITHOUT Location Data
UBSAN_HANDLER_SIMPLE(__ubsan_handle_pointer_overflow, void *data, void *base, void *result)
UBSAN_HANDLER_SIMPLE(__ubsan_handle_builtin_unreachable, void)
UBSAN_HANDLER_SIMPLE(__ubsan_handle_invalid_builtin, void)

#pragma clang diagnostic pop

#endif // ENABLE_LEA_UBSAN
