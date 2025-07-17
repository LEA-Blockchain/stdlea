CC := clang

STDLEA_MK_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

STDLEA_INCLUDE := -I$(STDLEA_MK_DIR)include

CFLAGS_BASE := --target=wasm32-unknown-unknown -ffreestanding -nostdlib -Wl,--no-entry -Wall -Wextra -pedantic

CFLAGS_WASM_FEATURES := -mbulk-memory -msign-ext -mmultivalue -flto

ifeq ($(ENABLE_UBSEN),1)
  ifneq ($(MAKECMDGOALS),clean)
    UBSEN_WARNING := $(shell printf '\033[31;1mUBSan is enabled. This will directly increase the program size and may impact performance.\033[0m\n' 1>&2)
  endif

STDLEA_SECURITY_CFLAGS := \
  -fsanitize=undefined \
  -fno-sanitize=alignment \
  -mexec-model=reactor \
  -mno-sign-ext \
  -DENABLE_LEA_UBSAN
endif

STDLEA_WARNING_CFLAGS := -Wall -Wextra -Wpedantic -Werror

SRCS := $(wildcard $(STDLEA_MK_DIR)src/*.c)

HDRS := $(wildcard $(STDLEA_MK_DIR)include/*.h $(STDLEA_MK_DIR)include/feature/*.h)

CFLAGS := ${CFLAGS_BASE} $(CFLAGS_WASM_FEATURES) $(STDLEA_SECURITY_CFLAGS) ${STDLEA_WARNING_CFLAGS} ${STDLEA_INCLUDE} -D__lea__

# --- Optional Compiler Flags ---
ifeq ($(ENABLE_LEA_LOG), 1)
CFLAGS += -DENABLE_LEA_LOG
endif
ifeq ($(ENABLE_LEA_FMT), 1)
CFLAGS += -DENABLE_LEA_FMT
endif
