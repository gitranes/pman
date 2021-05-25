#include "common/string.h"

#include <string.h>

bool string_check_chars(
    const char* str, bool (*const is_valid)(const char ch))
{
    const size_t size = strlen(str);

    for (size_t i = 0; i < size; ++i)
    {
        if (!is_valid(str[i]))
        {
            return false;
        }
    }
    return true;
}
