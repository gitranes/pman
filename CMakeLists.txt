cmake_minimum_required(VERSION 3.16)

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

# -std=c11 and warnings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -pedantic -Werror)

# Clang compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set up clang-format
set(CLANG_FORMAT_BIN_NAME clang-format-11)
set(CLANG_FORMAT_EXCLUDE_PATTERNS  "build/" ${CMAKE_BINARY_DIR})
find_package(ClangFormat)

add_subdirectory(src)
add_subdirectory(tests)
