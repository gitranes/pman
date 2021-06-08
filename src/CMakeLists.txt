# Linting tools
set(CMAKE_C_CLANG_TIDY "clang-tidy")
set(CMAKE_C_INCLUDE_WHAT_YOU_USE "iwyu")
set(CMAKE_LINK_WHAT_YOU_USE TRUE)

add_executable(pman)

# Inih library

FetchContent_Declare(
        inih
        GIT_REPOSITORY https://github.com/benhoyt/inih
        GIT_TAG r53
)
FetchContent_MakeAvailable(inih)

# A bit hacky as inih is not directly supported by FetchContent or FindModule
# TODO: disabling clang-tidy other way?
set(CMAKE_C_CLANG_TIDY "")

target_include_directories(pman SYSTEM PRIVATE ${inih_SOURCE_DIR})
add_library(inih ${inih_SOURCE_DIR}/ini.c)
set_target_properties(inih PROPERTIES COMPILE_FLAGS "")

set(CMAKE_C_CLANG_TIDY "clang-tidy")

# OpenSSL
find_package(OpenSSL REQUIRED)

# pman

target_link_libraries(pman OpenSSL::Crypto inih)

# Address sanitization with DebugAsan builds
target_link_options(
        pman
        PRIVATE
        "$<$<CONFIG:DebugAsan>:-fsanitize=address,undefined>"
)

target_include_directories(pman PRIVATE ../include)
target_sources(
        pman
        PRIVATE

        main.c

        auth/auth.c

        cmd/cmd.c
        cmd/drive.c
        cmd/impl.c
        cmd/setup.c

        common/buffer_view.c
        common/constants.c
        common/error_msg.c
        common/file.c
        common/info_msg.c
        common/paths.c
        common/string.c

        db/entries/category.c
        db/entries/entry_manage.c
        db/entries/entry.c
        db/driver.c
        db/master_key.c
        db/metadata.c

        encrypt/algorithm.c
        encrypt/file/decrypt.c
        encrypt/file/encrypt.c
        encrypt/meta/crypt.c
        encrypt/meta/iv.c
        encrypt/meta/key.c
        encrypt/raw/cipher.c
        encrypt/raw/crypt_block.c
        encrypt/raw/crypt.c

        hash/hash.c
        hash/hasher.c

        input/prompt_msgs.c
        input/prompt_static.c
        input/prompt_yn.c
        input/prompt_verify.c
        input/prompt_dynamic.c
        input/term.c

        logging/terminal_logger.c

        options/options.c
        options/parse.c

        persistence/config.c
        persistence/login_cache.c
)
