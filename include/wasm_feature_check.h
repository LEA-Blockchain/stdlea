#ifndef WASM_FEATURE_CHECK_H
#define WASM_FEATURE_CHECK_H

#if defined(ENV_WASM_MVP)
#ifdef __wasm_nontrapping_fptoint__
#error \
    "Wasm feature 'nontrapping-fptoint' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_nontrapping_fptoint__ defined)"
#endif
#ifdef __wasm_sign_ext__
#error "Wasm feature 'sign-ext' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_sign_ext__ defined)"
#endif
#ifdef __wasm_bulk_memory__
#error "Wasm feature 'bulk-memory' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_bulk_memory__ defined)"
#endif
#ifdef __wasm_reference_types__
#error "Wasm feature 'reference-types' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_reference_types__ defined)"
#endif
#ifdef __wasm_simd128__
#error "Wasm feature 'simd128' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_simd128__ defined)"
#endif
#ifdef __wasm_tail_call__
#error "Wasm feature 'tail-call' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_tail_call__ defined)"
#endif
#ifdef __wasm_multivalue__
#error "Wasm feature 'multivalue' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_multivalue__ defined)"
#endif
#ifdef __wasm_mutable_globals__
#error "Wasm feature 'mutable-globals' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_mutable_globals__ defined)"
#endif
#ifdef __wasm_multimemory__
#error "Wasm feature 'multimemory' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_multimemory__ defined)"
#endif
#ifdef __wasm_memory64__
#error "Wasm feature 'memory64' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_memory64__ defined)"
#endif
#ifdef __wasm_exception_handling__
#error "Wasm feature 'exception-handling' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_exception_handling__ defined)"
#endif
#ifdef __wasm_threads__
#error "Wasm feature 'threads' MUST BE DISABLED in ENV_WASM_MVP mode! (__wasm_threads__ defined)"
#endif

#elif defined(ENV_WASM_VM)
#ifndef __wasm_nontrapping_fptoint__
#error \
    "Required Wasm feature 'nontrapping-fptoint' is NOT enabled in ENV_WASM_VM mode! (Macro __wasm_nontrapping_fptoint__ not defined)"
#endif
#ifndef __wasm_sign_ext__
#error "Required Wasm feature 'sign-ext' is NOT enabled in ENV_WASM_VM mode! (Macro __wasm_sign_ext__ not defined)"
#endif
#ifndef __wasm_bulk_memory__
#error \
    "Required Wasm feature 'bulk-memory' is NOT enabled in ENV_WASM_VM mode! (Macro __wasm_bulk_memory__ not defined)"
#endif
#ifndef __wasm_reference_types__
#error \
    "Required Wasm feature 'reference-types' is NOT enabled in ENV_WASM_VM mode! (Macro __wasm_reference_types__ not defined)"
#endif
#ifndef __wasm_simd128__
#error "Required Wasm feature 'simd128' is NOT enabled in ENV_WASM_VM mode! (Macro __wasm_simd128__ not defined)"
#endif
#ifndef __wasm_tail_call__
#error "Required Wasm feature 'tail-call' is NOT enabled in ENV_WASM_VM mode! (Macro __wasm_tail_call__ not defined)"
#endif
#ifndef __wasm_multivalue__
#error "Required Wasm feature 'multivalue' is NOT enabled in ENV_WASM_VM mode! (Macro __wasm_multivalue__ not defined)"
#endif

/* --- Unwanted Wasm Features (Error if macro IS defined) --- */
#ifdef __wasm_mutable_globals__
#error \
    "Unwanted Wasm feature 'mutable-globals' IS enabled in ENV_WASM_VM mode! (Macro __wasm_mutable_globals__ defined). Use -mno-mutable-globals or similar."
#endif
#ifdef __wasm_multimemory__
#error \
    "Unwanted Wasm feature 'multimemory' IS enabled in ENV_WASM_VM mode! (Macro __wasm_multimemory__ defined). Check compiler flags."
#endif
#ifdef __wasm_memory64__
#if defined(__wasm32__)
#error \
    "Unwanted Wasm feature 'memory64' IS enabled for wasm32 target in ENV_WASM_VM mode! (Macro __wasm_memory64__ defined). Check compiler flags."
#endif
#endif
#ifdef __wasm_exception_handling__
#error \
    "Unwanted Wasm feature 'exception-handling' IS enabled in ENV_WASM_VM mode! (Macro __wasm_exception_handling__ defined). Check compiler flags."
#endif
#ifdef __wasm_threads__
#error \
    "Unwanted Wasm feature 'threads' IS enabled in ENV_WASM_VM mode! (Macro __wasm_threads__ defined). Check compiler flags."
#endif

#else
#error \
    "Compiling for Wasm target (__wasm32__ or __wasm64__) requires either ENV_WASM_MVP or ENV_WASM_VM to be defined."

#endif // End of ENV_WASM_MVP / ENV_WASM_VM / #error checks

#endif // WASM_FEATURE_CHECK_H
