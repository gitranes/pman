#include "persistence/login_cache.h"

#include <common/macros.h>

#include <stdlib.h>

struct LoginCache* login_cache_init()
{
    return calloc(1, sizeof(struct LoginCache));
}

void login_cache_clean(struct LoginCache* cache)
{
    if (cache->file)
    {
        fclose(cache->file);
    }
    free(cache);
}

struct StringView login_cache_read(struct LoginCache* cache)
{
    // TODO: Implement
    UNUSED(cache);
    struct StringView result = {0};
    return result;
}
