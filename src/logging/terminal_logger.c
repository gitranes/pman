#include "logging/terminal_logger.h"

#include "common/macros.h"

#include <stdarg.h>
#include <stdio.h>

// Terminal logger does not init or closing, so these can be empty
static int term_log_init(struct Logger* self, void* data);
static void term_log_close(struct Logger* self);

static void term_log_plain(const struct Logger* self, const char* fmt, ...);
static void term_log_info(const struct Logger* self, const char* fmt, ...);
static void term_log_error(const struct Logger* self, const char* fmt, ...);

static const char* const INFO_PREFIX = "[INFO]: ";
static const char* const ERROR_PREFIX = "[ERROR]: ";

const struct Logger TERMINAL_LOGGER = {
    .interface = (struct LoggerInterface) {
        .init = term_log_init,
        .close = term_log_close,
        .plain = term_log_plain,
        .info = term_log_info,
        .error = term_log_error,
    },
    .data = NULL
};

static int term_log_init(struct Logger* const self, void* data)
{
    UNUSED(self);
    UNUSED(data);
    return 0;
}
static void term_log_close(struct Logger* const self)
{
    UNUSED(self);
}

void term_log_plain(const struct Logger* self, const char* fmt, ...)
{
    UNUSED(self);
    va_list args;
    va_start(args, fmt);

    vprintf(fmt, args);

    va_end(args);
}

static void term_log_info(
    const struct Logger* const self, const char* fmt, ...)
{
    UNUSED(self);

    va_list args;
    va_start(args, fmt);

    puts(INFO_PREFIX);
    vprintf(fmt, args);

    va_end(args);
}

static void term_log_error(
    const struct Logger* const self, const char* fmt, ...)
{
    UNUSED(self);

    va_list args;
    va_start(args, fmt);

    puts(ERROR_PREFIX);
    vprintf(fmt, args);

    va_end(args);
}
