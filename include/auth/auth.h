#pragma once

struct LoginCache;
struct DbDriver;

/**
 * First searches the login cache, and if unsuccessful prompts the user for
 * the password. Passwords are always verified through Database db.
 * @param db_driver - Driver that has an open database
 * @param cache     - Points to a valid cache (can be NULL if no cache)
 * @return 0 if successful, else non-zero
 */
int auth_authenticate(
    struct DbDriver* db_driver, struct LoginCache* cache);
