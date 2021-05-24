#pragma once

#include "logging/logger.h"

extern const struct Logger TERMINAL_LOGGER;

// Macros for easier usage

#define TLOG_INFO(fmt, ...) \
    TERMINAL_LOGGER.interface.info(&TERMINAL_LOGGER, fmt, __VA_ARGS__)
#define TLOG_ERROR(fmt, ...) \
    TERMINAL_LOGGER.interface.info(&TERMINAL_LOGGER, fmt, __VA_ARGS__)
