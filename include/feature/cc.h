#ifndef CC_H
#define CC_H

#if !defined(__clang__)
#error "Use CLANG for LEA"
#endif

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#error "C11 or later is required for _Static_assert"
#endif

#if !defined(__has_builtin)
#error "__has_builtin is required to check for __builtin_types_compatible_p"
#endif

#if !__has_builtin(__builtin_types_compatible_p)
#error "__builtin_types_compatible_p is not supported by this compiler"
#endif

#endif // CC_H
