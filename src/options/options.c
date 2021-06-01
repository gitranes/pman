#include "options/options.h"

const struct OptionHolder* options_find(
    const struct Options* options, enum OptionTag tag)
{
    if (!options->holders[tag].is_active)
    {
        return NULL;
    }
    return &options->holders[tag];
}
