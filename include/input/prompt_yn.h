#pragma once

#include <stdbool.h>

enum YesNoStrategy
{
    YN_EMPTY_RETRY,
    YN_EMPTY_NO,
    YN_EMPTY_YES
};

bool prompt_yes_no(const char* prompt_msg, enum YesNoStrategy strategy);
