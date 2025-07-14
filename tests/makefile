CLANG := clang

ENABLE_UBSEN := 1
include ../stdlea.mk

CFLAGS_WASM_BASE := --target=wasm32-unknown-unknown -ffreestanding -nostdlib -Wl,--no-entry -Os -Wall -Wextra -pedantic
CFLAGS_WASM_FEATURES := -mbulk-memory -msign-ext -mmultivalue
CFLAGS_WASM := $(CFLAGS_WASM_BASE) $(CFLAGS_WASM_FEATURES) $(STDLEA_CFLAGS) -flto

CFLAGS_WASM_TEST_FMT := $(CFLAGS_WASM) -DSCTP_HANDLER_PROVIDED -DENABLE_LEA_FMT -DDISABLE_BUMP_ALLOCATOR
CFLAGS_WASM_TEST_LOG := $(CFLAGS_WASM) -DENABLE_LEA_LOG -DDISABLE_BUMP_ALLOCATOR
CFLAGS_WASM_TEST_MEMORY := $(CFLAGS_WASM) -DENABLE_LEA_FMT
CFLAGS_WASM_TEST_STRING := $(CFLAGS_WASM) -DENABLE_LEA_FMT -DDISABLE_BUMP_ALLOCATOR
CFLAGS_WASM_TEST_UBSEN := $(CFLAGS_WASM) -DENABLE_LEA_FMT -DDISABLE_BUMP_ALLOCATOR

SRC_TEST_FMT := test_fmt.c
SRC_TEST_LOG := test_log.c
SRC_TEST_MEMORY := test_memory.c
SRC_TEST_STRING := test_string.c
SRC_TEST_UBSEN := test_ubsen.c
ALL_SRCS_FOR_FORMAT := $(SRC_TEST_FMT) $(SRC_TEST_LOG) $(SRC_TEST_MEMORY) $(SRC_TEST_STRING) $(SRC_TEST_UBSEN)

TARGET_TEST_FMT := test_fmt.wasm
TARGET_TEST_LOG := test_log.wasm
TARGET_TEST_MEMORY := test_memory.wasm
TARGET_TEST_STRING := test_string.wasm
TARGET_TEST_UBSEN := test_ubsen.wasm
ALL_TARGETS := $(TARGET_TEST_FMT) $(TARGET_TEST_LOG) $(TARGET_TEST_MEMORY) $(TARGET_TEST_STRING) $(TARGET_TEST_UBSEN)

.PHONY: all clean format check-unicode test

#all: run

test: $(ALL_TARGETS)

#run: test
#	@./run_tests.sh

# Rule for the test modules
$(TARGET_TEST_FMT): format $(SRC_TEST_FMT) $(STDLEA_SRCS)
	@echo "Compiling and linking test module to $(TARGET_TEST_FMT)"
	$(CLANG) $(CFLAGS_WASM_TEST_FMT) $(SRC_TEST_FMT) $(STDLEA_SRCS) -o $(TARGET_TEST_FMT)
	@echo "Build complete: $@"

$(TARGET_TEST_LOG): format $(SRC_TEST_LOG) $(STDLEA_SRCS)
	@echo "Compiling and linking test module to $(TARGET_TEST_LOG)"
	$(CLANG) $(CFLAGS_WASM_TEST_LOG) $(SRC_TEST_LOG) $(STDLEA_SRCS) -o $(TARGET_TEST_LOG)
	@echo "Build complete: $@"

$(TARGET_TEST_MEMORY): format $(SRC_TEST_MEMORY) $(STDLEA_SRCS)
	@echo "Compiling and linking test module to $(TARGET_TEST_MEMORY)"
	$(CLANG) $(CFLAGS_WASM_TEST_MEMORY) $(SRC_TEST_MEMORY) $(STDLEA_SRCS) -o $(TARGET_TEST_MEMORY)
	@echo "Build complete: $@"

$(TARGET_TEST_STRING): format $(SRC_TEST_STRING) $(STDLEA_SRCS)
	@echo "Compiling and linking test module to $(TARGET_TEST_STRING)"
	$(CLANG) $(CFLAGS_WASM_TEST_STRING) $(SRC_TEST_STRING) $(STDLEA_SRCS) -o $(TARGET_TEST_STRING)
	@echo "Build complete: $@"

$(TARGET_TEST_UBSEN): format $(SRC_TEST_UBSEN) $(STDLEA_SRCS)
	@echo "Compiling and linking test module to $(TARGET_TEST_UBSEN)"
	$(CLANG) $(CFLAGS_WASM_TEST_UBSEN) $(SRC_TEST_UBSEN) $(STDLEA_SRCS) -o $(TARGET_TEST_UBSEN)
	@echo "Build complete: $@"

clean:
	@echo "Removing build artifacts..."
	rm -f $(ALL_TARGETS) *.o

format: check-unicode
	@echo "Formatting source files..."
	@clang-format -i $(ALL_SRCS_FOR_FORMAT)

check-unicode:
	@echo "Checking for non-ASCII characters..."
	@if grep --color=auto -P -n "[^\x00-\x7F]" $(ALL_SRCS_FOR_FORMAT) > /dev/null; then \
		echo "Unicode characters detected in source files!"; \
		grep --color=always -P -n "[^\x00-\x7F]" $(ALL_SRCS_FOR_FORMAT); \
		exit 1; \
	fi
