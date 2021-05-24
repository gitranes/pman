#include "auth/auth.h"

int auth_authenticate(
    struct MasterKey* const empty_key,
    struct LoginCache* const cache,
    struct DbDriver* const db_driver)
{
    return 0;
}

struct MasterKey* auth_master_key_init()
{
    return 0;
}

void auth_master_key_clean(struct MasterKey* const key)
{
}
