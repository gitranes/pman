#pragma once

#include <stdbool.h>

bool string_check_chars(
    const char* str, bool (*is_valid)(const char ch));
