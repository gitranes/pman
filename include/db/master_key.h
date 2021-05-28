#pragma once

#include "common/buffer_view.h"

#include <stdbool.h>

struct MasterKey
{
    struct ByteView view;
    bool verified;
};

struct MasterKey* db_master_key_init();
void db_master_key_clean(struct MasterKey* key);
