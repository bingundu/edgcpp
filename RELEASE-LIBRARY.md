# EDG callable front-end library (Linux x86-64)

The CMake option `EDG_BUILD_CALLABLE_LIBRARY=ON` enables a static archive of
the EDG compiler *front end*, separate from the C++ runtime in `lib_src/`.
The normal `cpfe` and `cpfe-cp` executable targets are unchanged.

## Build from a source checkout

On a Linux x86-64 host with GCC, Ninja, CMake and Python (and the repository's
`edg-cmakedef` tool available), run:

```sh
cmake --preset linux-gcc-release -DEDG_BUILD_LIB=OFF -DEDG_BUILD_CALLABLE_LIBRARY=ON
cmake --build build/gcc-release --target edgcpfe --parallel 2
```

The archive is at `build/gcc-release/lib/libedgcpfe.a`.
The release workflow performs these steps in the project's existing CI
container and packages the archive with `include/edgcpfe.h` and the license.

## Link from another C++ application

```sh
g++ -std=c++14 -I/path/to/package/include your_program.cpp \
  /path/to/package/lib/libedgcpfe.a -o your_program
```

The supported entry point in this distribution is:

```cpp
#include "edgcpfe.h"

// Call edg::edg_main(argc, argv) with EDG front-end command-line arguments.
```

The EDG front end is compiled with `MAKE_FRONT_END_CALLABLE=1`; instead of
defining a program `main()`, it exports `edg::edg_main(int, char**)`.
Consult the upstream front-end documentation for initialization, command-line
options, and integration with custom back ends. This package does not provide
a stable public API for the internal IL structures.

This is a GNU/Linux x86-64 C++ static archive, **not** an architecture-neutral
binary or a C ABI. Consumers should use a compatible Linux/glibc/C++ ABI.
Release archive objects are compiled without LTO for easier downstream linking.
The link smoke test verifies that an independent C++ program resolves the
callable entry point with an ordinary g++ link command.

## Tag builds

Pushing a tag named `edgcpfe-v*` that points at a commit containing
`.github/workflows/release-edgcpfe.yml` triggers the workflow. A successful
tag build uploads a versioned archive as a GitHub Actions artifact and
publishes the archive as a GitHub Release asset. Branch pushes to
`ci/x86_64-callable-release` build and link-test the archive without publishing.
