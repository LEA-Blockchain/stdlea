# Compiler and flags
CLANG := clang
AR := llvm-ar

# WASM flags
CFLAGS_WASM_BASE = --target=wasm32 -ffreestanding -nostdlib -Iinclude -flto -Os
CFLAGS_WASM_MVP = $(CFLAGS_WASM_BASE) -DENV_WASM_MVP
CFLAGS_WASM_VM = $(CFLAGS_WASM_BASE) -DENV_WASM_VM \
                  -mnontrapping-fptoint -mbulk-memory -msign-ext -msimd128 \
                  -mtail-call -mreference-types -matomics -mmultivalue \
                  -Xclang -target-abi -Xclang experimental-mv

# Source and object files
SRC := $(wildcard src/*.c)
OBJ_VM := $(SRC:.c=.vm.bc)
OBJ_MVP := $(SRC:.c=.mvp.bc)

# Library names
LIB_VM := libstdlea-vm.a
LIB_MVP := libstdlea-mvp.a

# --- Installation ---
PREFIX ?= /usr/local
INCLUDEDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib

# Phony targets
.PHONY: all clean docs clean-docs install uninstall

all: $(LIB_MVP) $(LIB_VM)

# Build the Lea virtual machine library
$(LIB_VM): $(OBJ_VM)
	$(AR) rcs $@ $^

# Build the Lea MVP library
$(LIB_MVP): $(OBJ_MVP)
	$(AR) rcs $@ $^

# Compile .c to .vm.bc
src/%.vm.bc: src/%.c include/stdlea.h
	$(CLANG) $(CFLAGS_WASM_VM) -c $< -o $@

# Compile .c to .mvp.bc
src/%.mvp.bc: src/%.c include/stdlea.h
	$(CLANG) $(CFLAGS_WASM_MVP) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJ_VM) $(OBJ_MVP) $(LIB_VM) $(LIB_MVP)

# Generate documentation
docs:
	doxygen Doxyfile

# Clean up documentation
clean-docs:
	rm -rf docs

# --- Installation ---

install: all
	@echo "Installing headers to $(DESTDIR)$(INCLUDEDIR)/stdlea..."
	install -d $(DESTDIR)$(INCLUDEDIR)/stdlea
	install -m 644 include/* $(DESTDIR)$(INCLUDEDIR)/stdlea/
	@echo "Installing libraries to $(DESTDIR)$(LIBDIR)..."
	install -d $(DESTDIR)$(LIBDIR)
	install -m 644 $(LIB_MVP) $(LIB_VM) $(DESTDIR)$(LIBDIR)/

uninstall:
	@echo "Uninstalling stdlea..."
	rm -rf $(DESTDIR)$(INCLUDEDIR)/stdlea
	rm -f $(DESTDIR)$(LIBDIR)/$(LIB_MVP)
	rm -f $(DESTDIR)$(LIBDIR)/$(LIB_VM)
	@echo "Uninstall complete."

