#pragma once

#include "common/buffer_view.h"

#include <stdalign.h>
#include <stdbool.h>

struct MasterKey
{
    struct BufferView key;
    bool verified;
};

struct LoginCache;
struct DbDriver;

int auth_authenticate(
    struct MasterKey* empty_key,
    struct LoginCache* cache,
    struct DbDriver* db_driver);

struct MasterKey* auth_master_key_init();
void auth_master_key_clean(struct MasterKey* key);
