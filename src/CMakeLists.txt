set(CMAKE_C_CLANG_TIDY "clang-tidy;--checks=*")

add_executable(pman main.c)

# Address sanitization with debug builds
target_link_options(
    pman
    PRIVATE
    "$<$<CONFIG:DEBUG>:-fsanitize=address,undefined>"
)
