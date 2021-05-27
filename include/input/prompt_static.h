#pragma once

char* prompt_static_text(const char* prompt_msg);
char* prompt_static_password(const char* prompt_msg);
char* prompt_static_password_twice(
    const char* prompt_msg, const char* verify_prompt);
