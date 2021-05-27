#pragma once

#include "common/buffer_view.h"

#include <stdbool.h>
#include <stdio.h>

struct LoginCache
{
    FILE* file;
    bool cache_enabled;
};

struct LoginCache* login_cache_init();
void login_cache_clean(struct LoginCache* cache);

struct StringView login_cache_read(struct LoginCache* cache);
int login_cache_write(struct LoginCache* cache, struct StringView* key);
