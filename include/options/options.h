#pragma once

#include "cmd/cmd.h"

#include <stddef.h>

enum OptionTag
{
    OPT_ECHO = 0,
    OPT_DB_PATH,
    OPT_CATEGORY,
    OPT_CLIP,
    OPT_FORCE
};

struct OptionHolder
{
    enum OptionTag tag;
    char** args;
};

struct OptionHolderArray
{
    struct OptionHolder* buf;
    size_t size;
};

struct Options
{
    enum Command cmd;
    struct OptionHolderArray holders;
    char** args;
};

struct OptionHolder* options_find(
    const struct Options* options, enum OptionTag tag);
