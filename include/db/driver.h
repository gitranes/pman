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
    struct ByteView raw_data;
};

struct DbDriver* db_drive_init();
void db_drive_clean(struct DbDriver* driver);

static inline bool db_drive_db_is_open(struct DbDriver* driver)
{
    return driver->fp;
}

int db_drive_new_db(
    struct DbDriver* driver,
    const char* db_path,
    struct StringView master_pass);

int db_drive_open_db(struct DbDriver* driver, const char* db_path);
void db_drive_close_db(struct DbDriver* driver);

int db_drive_verify_key(
    struct DbDriver* driver,
    struct MasterKey* empty_key,
    struct StringView master_pass);

int db_drive_read_db_data(struct DbDriver* driver, struct MasterKey* key);
int db_drive_update_db_data(struct DbDriver* driver);
