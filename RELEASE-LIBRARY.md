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

## Upstream-based release

This release branch is based directly on upstream `edgcpp/compiler` commit
`177765bb773156a7f3d77fa989f11bc9e1715cfe`. No private EDG
lowering hooks are included. The fork's `main` tracks that upstream
snapshot exactly; the historical fork tip (including hooks) is backed up at
`archive/pre-upstream-sync-with-edg-hooks-20261114`.

Pushing `ci/upstream-callable-edg-release-20261114` triggers an x86-64
archive build, link smoke test, and (on success) one immutable release named
`edgcpfe-v2026.11.14-upstream-177765bb7731`. To avoid accidentally
overwriting different binaries under the same upstream version, the release
job fails if that tag already has a GitHub Release. The source SHA of the
actual packaging commit is recorded inside `BUILD-INFO.txt`.
