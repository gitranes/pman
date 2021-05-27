#pragma once

#include "common/constants.h"

enum BufferSizes
{
    PROMPT_MAX_STATIC_TEXT_LEN = CST_MAX_INPUT_SIZE + 1,
    PROMPT_MAX_STATIC_PW_LEN = CST_MAX_PASSWORD_LEN + 1
};

char* prompt_static_text(const char* prompt_msg);
char* prompt_static_password(const char* prompt_msg);
char* prompt_static_password_twice(
    const char* prompt_msg, const char* verify_prompt);
