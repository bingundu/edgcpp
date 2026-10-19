# Part of the EDG Compiler Project, under the Apache License v2.0 with LLVM
# Exceptions.
# See https://edgcpp.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# List of all historic files for the front end.
set(ALL_C_FILES "${CORE_FRONT_END_SOURCE_FILES}")

# Load the specified macro configuration for cpfe.
macro_configuration_to_compile_definitions("cpfe")

# compile .c files as C++
set_source_files_properties(${ALL_C_FILES} PROPERTIES LANGUAGE CXX)

# Set the name of the primary binary (i.e., the one that actually contains
# the front end source code).
set(primary_front_end_target "cpfe")

if($CACHE{MULTI_THREADED_DAEMON})
  # Add the daemon executable.
  add_executable(cpfe-daemon ${ALL_C_FILES} ${TXT_FILES})
  add_dependencies(cpfe-daemon error-info)

  # Additionally add the client executable.
  set(cli_src_file ${CMAKE_SOURCE_DIR}/util/cfe_daemon_client.c)
  set_source_files_properties(${cli_src_file} PROPERTIES LANGUAGE CXX)
  add_executable(cpfe ${cli_src_file})

  # Allow the daemon client to see the project source directory in its include
  # path.
  target_include_directories(cpfe PRIVATE ${PROJECT_SOURCE_DIR})

  # Update the primary binary name.
  set(primary_front_end_target "cpfe-daemon")

  # Make the client depend on the daemon so the cpfe build target still
  # does the expected thing.
  add_dependencies(cpfe cpfe-daemon)
else()
  # Add the main cpfe executable.
  add_executable(cpfe ${ALL_C_FILES} ${TXT_FILES})
  add_dependencies(cpfe error-info)
endif()

add_dependencies(cpfe-all cpfe)

# Expose softfloat.
add_softfloat_to_target("${primary_front_end_target}")

# Build an embeddable front-end archive without changing the default executables.
# EDG's callable mode replaces main() with edg::edg_main(int, char **).
option(EDG_BUILD_CALLABLE_LIBRARY
       "Build a static, callable EDG C/C++ front-end library" OFF)

if(EDG_BUILD_CALLABLE_LIBRARY)
  add_library(edgcpfe STATIC ${ALL_C_FILES} ${TXT_FILES})
  add_dependencies(edgcpfe error-info)

  # Release executables use GCC/Clang LTO; distributing LTO objects ties a
  # static library to a specific compiler plugin/version.  Emit native x86-64
  # object code instead so ordinary g++ link commands can consume the archive.
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
     CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(edgcpfe PRIVATE "$<$<CONFIG:Release>:-fno-lto>")
  endif()

  target_compile_definitions(edgcpfe PRIVATE MAKE_FRONT_END_CALLABLE=1)
  add_softfloat_to_target(edgcpfe)
endif()
