FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity
    GIT_TAG v2.5.2
)
FetchContent_MakeAvailable(unity)

add_executable(pman_tests tests.c)
target_link_libraries(pman_tests unity)

add_test(test_pman pman_tests)
