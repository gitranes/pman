#include "input/prompt_msgs.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* const PROMPT_AUTH_MASTER_PW_FMT =
    "Enter master password for database '%s': ";

const char* const PROMPT_NEW_DB_MASTER_PW =
    "Enter the master password: ";
const char* const PROMPT_NEW_DB_VERIFY_MASTER_PW =
    "Enter the master password again: ";

const char* const PROMPT_ADD_ENTRY_USERNAME_FMT =
    "Enter username for entry '%s': ";
const char* const PROMPT_ADD_ENTRY_PASSWORD_FMT =
    "Enter password for entry '%s': ";
const char* const PROMPT_ADD_VERIFY_ENTRY_PASSWORD_FMT =
    "Enter password for entry '%s' again: ";

char* prompt_fill_fmt(const char* fmt, const char* arg)
{
    const size_t result_str_size = strlen(fmt) + strlen(arg) + 1; // null byte
    char* const result_str = calloc(1, result_str_size);

    snprintf(result_str, result_str_size, fmt, arg);
    return result_str;
}
