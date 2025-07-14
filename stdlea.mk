STDLEA_MK_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

STDLEA_INCLUDE := -I$(STDLEA_MK_DIR)include

# Security-related CFLAGS for WebAssembly builds
# -fsanitize=undefined     : Catch undefined behavior at runtime
# -flto                    : Enable link-time optimization (required by CFI)
# -mexec-model=reactor     : Safer WASM module model for embedding
# -mno-sign-ext            : Prevent unexpected sign-extension of integers
ifeq ($(ENABLE_UBSEN),1)
STDLEA_SECURITY_CFLAGS := \
  -fsanitize=undefined \
  -flto \
  -mexec-model=reactor \
  -mno-sign-ext \
  -DENABLE_LEA_UBSAN
endif

# Warning and enforcement flags
STDLEA_WARNING_CFLAGS := \
  -Wall -Wextra -Wpedantic -Werror

STDLEA_CFLAGS := $(STDLEA_INCLUDE) $(STDLEA_WARNING_CFLAGS) $(STDLEA_SECURITY_CFLAGS)

STDLEA_SRCS := $(wildcard $(STDLEA_MK_DIR)src/*.c)

STDLEA_HDRS := $(wildcard $(STDLEA_MK_DIR)include/*.h $(STDLEA_MK_DIR)include/feature/*.h)
