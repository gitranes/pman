set(CMAKE_C_CLANG_TIDY "clang-tidy")
set(CMAKE_C_INCLUDE_WHAT_YOU_USE "iwyu")
set(CMAKE_LINK_WHAT_YOU_USE TRUE)

add_executable(pman main.c)

# Address sanitization with debug builds
target_link_options(
    pman
    PRIVATE
    "$<$<CONFIG:DEBUG>:-fsanitize=address,undefined>"
)
