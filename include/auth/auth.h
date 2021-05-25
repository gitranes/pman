#pragma once

#include "encrypt/algorithm.h"

#include "common/buffer_view.h"

#include <stdalign.h>
#include <stdbool.h>

struct MasterKey
{
    struct ByteView view;
    bool verified;
};

struct LoginCache;
struct DbDriver;

/**
 * First searches the login cache, and if unsuccessful prompts the user for
 * the password. Passwords are always verified through Database db.
 * @param empty_key - To be filled with the decrypt key, if successful
 * @param db_driver - Driver that is ready to verify keys
 * @param cache     - Points to a valid cache (can be NULL if no cache)
 * @return 0 if successful, else non-zero
 */
int auth_authenticate(
    struct MasterKey* empty_key,
    struct DbDriver* db_driver,
    struct LoginCache* cache);

struct MasterKey* auth_master_key_init(size_t size);
void auth_master_key_clean(struct MasterKey* key);
