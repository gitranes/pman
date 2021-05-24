set(CMAKE_C_CLANG_TIDY "clang-tidy")
set(CMAKE_C_INCLUDE_WHAT_YOU_USE "iwyu")
set(CMAKE_LINK_WHAT_YOU_USE TRUE)

add_executable(pman)

target_include_directories(pman PRIVATE ../include)
target_sources(
        pman
        PRIVATE

        main.c

        auth/auth.c

        cmd/drive.c
        cmd/impl.c

        common/buffer_view.c

        db/entries/category.c
        db/entries/entry_manage.c
        db/entries/entry.c
        db/driver.c
        db/metadata.c

        encrypt/file/decrypt.c
        encrypt/file/encrypt.c
        encrypt/raw/crypt_block.c
        encrypt/raw/crypt_meta.c
        encrypt/raw/decrypt.c
        encrypt/raw/encrypt.c

        hash/hash.c
        hash/hasher.c

        input/prompt_static.c
        input/prompt_yn.cpp
        input/prompt_verify.cpp
        input/prompt_dynamic.cpp

        logging/terminal_logger.c

        options/options.c
        options/parse.c

        persistence/config.c
        persistence/login_cache.c
)

# Address sanitization with debug builds
target_link_options(
    pman
    PRIVATE
    "$<$<CONFIG:DEBUG>:-fsanitize=address,undefined>"
)
