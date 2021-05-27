#pragma once

#include "options/options.h"

enum ParseStatus
{
    PARSE_OK = 0,
    PARSE_FAIL = 1
};

struct ParseResult
{
    enum ParseStatus status;
    struct Options* options;
};

int options_parse(
    struct ParseResult* result,
    int argc,
    char* argv[]);

struct ParseResult* options_parse_result_init();
void options_parse_result_clean();
