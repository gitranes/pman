#include "db/driver.h"
#include "db/metadata.h"
#include <db/entries/entry_manage.h>

#include "common/macros.h"

#include <stdio.h>
#include <stdlib.h>

enum DefaultSizes
{
    DB_DEFAULT_RAW_SIZE = 4096
};

static int db_calculate_key(
    struct DbDriver* driver,
    struct MasterKey* empty_key,
    struct StringView master_pass);
static int db_decrypt_data(struct DbDriver* driver);
static int db_set_integrity_hash(struct DbDriver* driver);

struct DbDriver* db_drive_init()
{
    struct DbDriver* const driver = malloc(sizeof(*driver));
    driver->fp = NULL;
    driver->metadata = db_meta_init();
    driver->entries = db_entries_init();
    driver->raw_data = (struct ByteView){
        .buf = malloc(DB_DEFAULT_RAW_SIZE),
        .size = DB_DEFAULT_RAW_SIZE
    };

    return driver;
}

void db_drive_clean(struct DbDriver* const driver)
{
    db_drive_close_db(driver);

    db_meta_clean(driver->metadata);
    db_entries_clean(driver->entries);

    free(driver->raw_data.buf);
    free(driver);
}

int db_drive_open_db(struct DbDriver* driver, const char* db_path)
{
    driver->fp = fopen(db_path, "r");
    if (!db_drive_db_is_open(driver))
    {
        return -1;
    }
    if (db_meta_read(driver->metadata, driver->fp))
    {
        return -2;
    }
}

void db_drive_close_db(struct DbDriver* const driver)
{
    if (!db_drive_db_is_open(driver))
    {
        return;
    }
}

int db_drive_verify_key(
    struct DbDriver* driver,
    struct MasterKey* empty_key,
    struct StringView master_pass)
{
    if (db_meta_calculate_key(driver->metadata, empty_key, master_pass))
    {
        return -1;
    }
}

static int db_decrypt_data(struct DbDriver* driver)
{
    return 0;
}

static int db_set_integrity_hash(struct DbDriver* driver)
{
    return 0;
}
