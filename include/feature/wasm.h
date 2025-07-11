#ifndef WASM_H
#define WASM_H

#ifndef __wasm32__
#error "LEA VM requires the target platform to be wasm32."
#endif

/**
 * @file wasm.h.h
 * @brief Compile-time checks for WASM features supported by the LEA VM.
 *
 * This header validates that the compiler settings match the feature set
 * defined in LIP 8. It ensures that enabled features are deterministic and
 * secure, preventing consensus failures.
 */

//==============================================================================
// Required LEA VM Features
// These features are stable and deterministic. Compilation will fail if they
// are not enabled.
//==============================================================================

#ifndef __wasm_bulk_memory__
#error "LEA VM requires 'bulk-memory'. Enable with the '-mbulk-memory' compiler flag."
#endif

#ifndef __wasm_sign_ext__
#error "LEA VM requires 'sign-ext'. Enable with the '-msign-ext' compiler flag."
#endif

#ifndef __wasm_multivalue__
#error "LEA VM requires 'multivalue'. Enable with the '-mmultivalue' compiler flag."
#endif

//==============================================================================
// Prohibited LEA VM Features
// These features are disabled to prevent non-determinism and security risks.
// Compilation will fail if they are enabled.
//==============================================================================

#ifdef __wasm_simd128__
#error "LEA VM forbids 'simd128' due to non-deterministic behavior across CPUs. Disable with '-mno-simd128'."
#endif

#ifdef __wasm_threads__
#error "LEA VM forbids 'threads' and atomics. Smart contracts must be single-threaded. Disable with '-mno-atomics'."
#endif

#ifdef __wasm_nontrapping_fptoint__
#error \
    "LEA VM forbids 'nontrapping-fptoint'. Invalid conversions must trap to ensure determinism. Disable with '-mno-nontrapping-fptoint'."
#endif

// --- Prohibited Experimental or Complex Features ---

#ifdef __wasm_reference_types__
#error \
    "LEA VM forbids 'reference-types' as it is not considered secure enough for blockchain use. Disable with '-mno-reference-types'."
#endif

#ifdef __wasm_tail_call__
#error \
    "LEA VM forbids 'tail-call' due to its experimental nature and unclear stack behavior. Disable with '-mno-tail-call'."
#endif

#ifdef __wasm_exception_handling__
#error "LEA VM forbids 'exception-handling'. It is an experimental proposal not suitable for consensus."
#endif

#endif // WASM_H
