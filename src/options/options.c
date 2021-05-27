#include "options/options.h"

struct OptionHolder* options_find(
    const struct Options* options, enum OptionTag tag)
{
    for (size_t i = 0; i < options->holders.size; ++i)
    {
        struct OptionHolder* const holder = &options->holders.buf[i];
        if (holder->tag == tag)
        {
            return holder;
        }
    }
    return NULL;
}
