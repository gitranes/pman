#pragma once

#include "options/options.h"

enum ParseStatus
{
    PARSE_OK = 0,

    PARSE_NO_CMD,
    PARSE_UNRECOGNIZED_OPTION,
    PARSE_MISSING_ARGUMENT,

    PARSE_UNSUPPORTED_CMD,
    PARSE_UNSUPPORTED_OPT
};

struct ParseResult
{
    enum ParseStatus status;
    struct Options* options;
};

struct ParseResult* options_parse_result_init();
void options_parse_result_clean(struct ParseResult* result);

int options_parse(
    struct ParseResult* result, int argc, char* argv[]);
