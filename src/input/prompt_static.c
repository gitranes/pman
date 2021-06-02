#include "input/prompt_static.h"
#include "input/term.h"

#include "common/error_msg.h"
#include "common/info_msg.h"

#include "logging/terminal_logger.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static char g_pw_buffer[PROMPT_MAX_STATIC_TEXT_LEN];
static char g_text_buffer[PROMPT_MAX_STATIC_PW_LEN];

static const size_t SCANF_MAX_DIGITS = 12;

static struct TerminalHandle* prompt_try_to_disable_echo();
static void prompt_scan_string(char* buffer, unsigned size);
void prompt_verify_pw(
    const char* prompt_msg, const char* verify_prompt, char* verify_buffer);

char* prompt_static_text(const char* prompt_msg)
{
    fputs(prompt_msg, stdout);
    prompt_scan_string(g_text_buffer, sizeof(g_text_buffer) - 1);
    return g_text_buffer;
}

char* prompt_static_password(const char* prompt_msg)
{
    struct TerminalHandle* const term = prompt_try_to_disable_echo();

    // puts does newline.. so use fputs
    fputs(prompt_msg, stdout);
    prompt_scan_string(g_pw_buffer, sizeof(g_pw_buffer) - 1);
    putchar('\n'); // Note: Disabled echo skips line break

    term_handle_clean(term);
    return g_pw_buffer;
}

char* prompt_static_password_twice(
    const char* prompt_msg, const char* verify_prompt)
{
    static char verify_buffer[PROMPT_MAX_STATIC_PW_LEN] = "";

    struct TerminalHandle* const term = prompt_try_to_disable_echo();
    prompt_verify_pw(prompt_msg, verify_prompt, verify_buffer);

    term_handle_clean(term);
    return g_pw_buffer;
}

void prompt_verify_pw(
    const char* prompt_msg,
    const char* verify_prompt,
    char* verify_buffer)
{
    // Prompt password twice and compare
    bool verify_ok = false;
    while (!verify_ok)
    {
        fputs(prompt_msg, stdout);
        prompt_scan_string(g_pw_buffer, sizeof(g_pw_buffer) - 1);
        putchar('\n'); // Note: Disabled echo wont show user line break

        fputs(verify_prompt, stdout);
        prompt_scan_string(verify_buffer, sizeof(verify_buffer) - 1);
        putchar('\n'); // Note: See above

        if (strcmp(g_pw_buffer, verify_buffer) == 0)
        {
            verify_ok = true;
        }
        else
        {
            TLOG_ERROR("%s\n", ERROR_PASSWORD_VERIFY);
        }
    }
}

void prompt_scan_string(char* buffer, unsigned size)
{
    char format[SCANF_MAX_DIGITS];
    snprintf(format, sizeof(format), "%%%ds", size - 1);
    scanf(format, buffer);
}

static struct TerminalHandle* prompt_try_to_disable_echo()
{
    struct TerminalHandle* const term = term_handle_init();
    if (!term || term_echo_off(term) != 0)
    {
        TLOG_INFO("%s\n", INFO_TERM_ECHO_FAIL);
        term_handle_clean(term);
        return NULL;
    }
    return term;
}
