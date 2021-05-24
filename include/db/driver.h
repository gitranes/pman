#pragma once

#include "common/buffer_view.h"

#include <stdio.h>

struct MasterKey;

struct DbMetadata;
struct DbEntries;

struct DbDriver
{
    FILE* fp;
    struct DbMetadata* metadata;
    struct DbEntries* entries;
};

struct DbDriver* db_drive_init();
void db_drive_clean(struct DbDriver* driver);

int db_drive_open_db(struct DbDriver* driver, const char* db_path);
void db_drive_close_db(struct DbDriver* driver);

int db_drive_verify_key(
    struct DbDriver* driver,
    struct MasterKey* key,
    struct StringView master_pass);
int db_drive_read_db_data(struct DbDriver* driver, struct MasterKey* key);
