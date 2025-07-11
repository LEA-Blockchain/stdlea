# Lea Standard Library (stdlea)

The official standard library (`stdlea`) for developing smart contracts on the Lea platform. It provides core types and functions optimized for the Lea WebAssembly (WASM) environment.

## Status: Beta

**Please be aware that `stdlea` is currently in beta.** While the core functionality is in place, APIs may still evolve.

## Features

*   **Core Types:** Standard C integer types, pointers, and boolean values.
*   **Memory Management:** A simple bump allocator (`malloc`, `memset`, `memcpy`, `memmove`, `memcmp`).
*   **String Formatting:** `printf` and `snprintf` for flexible string formatting.
*   **Logging:** A basic logging facility (`lea_log`).

## Building the Library

To build the `stdlea` library, you will need `clang` and `llvm-ar`. Once you have the prerequisites, run the following command:

```bash
make
```

This will produce the `libstdlea.a` library.

## Building the Documentation

The code is documented using Doxygen. To generate the HTML documentation, you will need `doxygen` installed. Then, run:

```bash
make docs
```

The documentation will be generated in the `docs/html` directory. You can open `docs/html/index.html` in your browser to view it.

## About

This project is part of the [LEA Project](https://getlea.org).

## Author

Developed by Allwin Ketnawang.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.