#pragma once

#include "common/buffer_view.h"

#include <stdbool.h>
#include <stdio.h>

struct MasterKey;

struct DbMetadata;
struct DbEntries;

struct DbDriver
{
    const char* db_path;
    FILE* fp;
    struct DbMetadata* metadata;
    struct DbEntries* entries;
    struct MasterKey* master_key;
    struct ByteView raw_data;
};

struct DbDriver* db_drive_init();
void db_drive_clean(struct DbDriver* driver);

int db_drive_new_db(
    struct DbDriver* driver,
    const char* db_path,
    struct StringView master_pass);

int db_drive_open_db(struct DbDriver* driver, const char* db_path);
void db_drive_close_db(struct DbDriver* driver);

static inline bool db_drive_db_is_open(struct DbDriver* driver)
{
    return driver->fp;
}

/**
 * Attempts to unlock the database with the given master password. Unlocking
 * the database allows the user to read and modify the data.
 * @param driver
 * @param master_pass
 * @return 0 on success, non-zero on fail
 */
int db_drive_unlock_db(struct DbDriver* driver, struct StringView master_pass);

int db_drive_read_db_data(struct DbDriver* driver);
int db_drive_update_db_data(struct DbDriver* driver);
