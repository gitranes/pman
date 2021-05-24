#include "db/driver.h"

struct DbDriver* db_drive_init()
{
    return NULL;
}

void db_drive_clean(struct DbDriver* const driver)
{
}

int db_drive_open_db(struct DbDriver* const driver, const char* db_path)
{
    return 0;
}

void db_drive_close_db(struct DbDriver* const driver)
{
}

int db_drive_verify_key(
    struct DbDriver* driver,
    struct MasterKey* key,
    struct StringView master_pass)
{
    return 0;
}

int db_drive_read_db_data(
    struct DbDriver* const driver,
    struct MasterKey* const key)
{
    return 0;
}
