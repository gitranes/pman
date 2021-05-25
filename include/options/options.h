#pragma once

#include "cmd/cmd.h"

#include "common/fixed_array.h"

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

struct Options
{
    enum Command cmd;
    struct FixedArray options; // array of OptionHolders
    char** args;
};

struct OptionHolder* options_find(
    const struct Options* options, enum OptionTag tag);
