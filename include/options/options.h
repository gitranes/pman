#pragma once

#include "cmd/cmd.h"

#include <stdbool.h>
#include <stddef.h>

// Acts as a tag and as an index
enum OptionTag
{
    OPT_ECHO = 0,
    OPT_DB_PATH,
    OPT_CATEGORY,
    OPT_CLIP,
    OPT_FORCE,
    OPT_MAXIMUM
};

enum Limits
{
    // Saves us from having to fiddle around with dynamically sized char**
    // Command is not included
    MAX_ARGS = 1
};

struct OptionHolder
{
    bool is_active;
    char* arg;
};

struct Options
{
    enum Command cmd;
    struct OptionHolder holders[OPT_MAXIMUM];
    char* args[MAX_ARGS]; // Note: points to argv, do not free
};

const struct OptionHolder* options_find(
    const struct Options* options, enum OptionTag tag);
