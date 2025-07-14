CLANG := clang
AR := llvm-ar

VERSION := 1.0.0

ENABLE_USB := 1

include stdlea.mk

.PHONY: all clean docs clean-docs install uninstall format check-unicode

all: format test

#test:
#	@echo "Running tests..."
#	@$(MAKE) -C tests

format: check-unicode
	@echo "Formatting source files..."
	clang-format -i $(STDLEA_SRCS) $(STDLEA_HDRS)

check-unicode:
	@echo "Checking for non-ASCII characters..."
	@if grep --color=auto -P -n "[^\x00-\x7F]" $(STDLEA_SRCS) $(STDLEA_HDRS) > /dev/null; then \
	    echo "❌ Unicode characters detected in source files!"; \
	    grep --color=always -P -n "[^\x00-\x7F]" $(STDLEA_SRCS) $(STDLEA_HDRS); \
	    exit 1; \
	fi

clean:
	@$(MAKE) -C tests clean

docs:
	doxygen Doxyfile

clean-docs:
	rm -rf docs