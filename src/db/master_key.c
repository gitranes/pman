#include "db/master_key.h"

#include <stdlib.h>

enum KeySizes
{
    DB_MASTER_KEY_SIZE = 64
};

struct MasterKey* db_master_key_init()
{
    struct MasterKey* const key = calloc(1, sizeof(*key));
    key->view.buf = calloc(1, DB_MASTER_KEY_SIZE);
    key->view.size = DB_MASTER_KEY_SIZE;
    return key;
}

void db_master_key_clean(struct MasterKey* key)
{
    free(key->view.buf);
    free(key);
}
