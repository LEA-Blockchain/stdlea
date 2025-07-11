CLANG := clang
AR := llvm-ar

VERSION := 1.0.0

CFLAGS_WASM = --target=wasm32 -ffreestanding -nostdlib -Iinclude -flto -Oz \
              -mbulk-memory -msign-ext -mmultivalue \
              -Wall -Wextra -Wpedantic -Werror \
              -Wshadow -Wconversion -Wsign-conversion -Wcast-align -Wunused -Wundef

SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

LIB := libstdlea.a
PC_FILE := stdlea.pc

PREFIX ?= /usr/local
INCLUDEDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib
PKGCONFIGDIR = $(LIBDIR)/pkgconfig

.PHONY: all clean docs clean-docs install uninstall format check-unicode

all: format $(LIB) $(PC_FILE)

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

%.o: %.c include/stdlea.h
	$(CLANG) $(CFLAGS_WASM) -c $< -o $@

$(PC_FILE): stdlea.pc.in
	sed -e "s|@prefix@|$(PREFIX)|" \
	    -e "s|@libdir@|$(LIBDIR)|" \
	    -e "s|@includedir@|$(INCLUDEDIR)|" \
	    -e "s|@version@|$(VERSION)|" \
	    $< > $@

format: check-unicode
	@echo "Formatting source files..."
	clang-format -i $(SRC) include/*.h

check-unicode:
	@echo "Checking for non-ASCII characters..."
	@if grep --color=auto -P -n "[^\x00-\x7F]" src/*.c include/*.h > /dev/null; then \
	    echo "❌ Unicode characters detected in source files!"; \
	    grep --color=always -P -n "[^\x00-\x7F]" src/*.c include/*.h; \
	    exit 1; \
	fi

clean:
	rm -f $(OBJ) $(LIB) $(PC_FILE)

docs:
	doxygen Doxyfile

clean-docs:
	rm -rf docs

install: all
	@echo "Installing headers to $(DESTDIR)$(INCLUDEDIR)/stdlea..."
	install -d $(DESTDIR)$(INCLUDEDIR)/stdlea
	install -d $(DESTDIR)$(INCLUDEDIR)/stdlea/feature
	install -m 644 include/*.h $(DESTDIR)$(INCLUDEDIR)/stdlea/
	install -m 644 include/feature/* $(DESTDIR)$(INCLUDEDIR)/stdlea/feature/
	@echo "Installing libraries to $(DESTDIR)$(LIBDIR)..."
	install -d $(DESTDIR)$(LIBDIR)
	install -m 644 $(LIB) $(DESTDIR)$(LIBDIR)/
	@echo "Installing pkg-config file to $(DESTDIR)$(PKGCONFIGDIR)..."
	install -d $(DESTDIR)$(PKGCONFIGDIR)
	install -m 644 $(PC_FILE) $(DESTDIR)$(PKGCONFIGDIR)/

uninstall:
	@echo "Uninstalling stdlea..."
	rm -rf $(DESTDIR)$(INCLUDEDIR)/stdlea
	rm -f $(DESTDIR)$(LIBDIR)/$(LIB)
	rm -f $(DESTDIR)$(PKGCONFIGDIR)/$(PC_FILE)
	@echo "Uninstall complete."
