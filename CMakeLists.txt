cmake_minimum_required(VERSION 3.16)
project(
    pman
    VERSION 0.1.0
    DESCRIPTION "A command line password manager"
    LANGUAGES C
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(FetchContent)
include(CTest)

# -std=c11 and warnings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_subdirectory(src)
add_subdirectory(tests)
