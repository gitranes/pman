cmake_minimum_required(VERSION 3.16)

if (WIN32)
    message(FATAL_ERROR "Windows in not yet supported.")
endif()
if (UNIX)
    # The project uses a few POSIX and GNU extensions:
    #   from POSIX: fileno
    #   from GNU: O_TMPFILE
    add_compile_definitions(_POSIX_C_SOURCE _GNU_SOURCE)
endif()

project(
    pman
    VERSION 0.1.0
    DESCRIPTION "A command line password manager"
    LANGUAGES C
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules
                      ${CMAKE_MODULE_PATH})

# I don't want the 50 useless targets that CTest brings
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)

include(FetchContent)
include(CTest)

# -std=c17 and warnings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -pedantic -Werror)

# Clang compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set up clang-format
set(CLANG_FORMAT_BIN_NAME clang-format)
set(CLANG_FORMAT_EXCLUDE_PATTERNS  "cmake-build-debug/" ${CMAKE_BINARY_DIR})
find_package(ClangFormat)

add_subdirectory(src)
add_subdirectory(tests)
