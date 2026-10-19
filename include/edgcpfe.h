/* Part of the EDG Compiler Project, under the Apache License v2.0 with
 * LLVM Exceptions. SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 * Entry point for the callable EDG front end built as libedgcpfe.a.
 * Use a C++14-or-later compiler with a compatible C++ ABI.
 */
#ifndef EDGCPFE_H
#define EDGCPFE_H

namespace edg {
int edg_main(int argc, char *argv[]);
}

#endif /* EDGCPFE_H */
