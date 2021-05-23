
#include "common/buffer_view.h"

#include <stdbool.h>

struct MasterKey
{
    struct BufferView key;
    bool verified;
};

struct LoginCache;
struct DbDriver;

int auth_authenticate(
    struct MasterKey* const empty_key,
    struct LoginCache* const cache,
    struct DbDriver* const db_driver
);

struct MasterKey* auth_master_key_init();
void auth_master_key_clean(struct MasterKey* const key);
