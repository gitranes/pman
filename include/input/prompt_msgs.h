#pragma once

extern const char* const PROMPT_AUTH_MASTER_PW_FMT;

extern const char* const PROMPT_NEW_DB_FILENAME;
extern const char* const PROMPT_NEW_DB_MASTER_PW;
extern const char* const PROMPT_NEW_DB_VERIFY_MASTER_PW;

extern const char* const PROMPT_ADD_ENTRY_USERNAME_FMT;
extern const char* const PROMPT_ADD_ENTRY_PASSWORD_FMT;
extern const char* const PROMPT_ADD_VERIFY_ENTRY_PASSWORD_FMT;

/**
 * One arg fmt filler. Variadic fill is a bit difficult to implement.
 * @param fmt - printf style fmt
 * @param arg
 * @return Allocated string (use free)
 */
char* prompt_fill_fmt(const char* fmt, const char* arg);
