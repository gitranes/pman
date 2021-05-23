
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
void db_drive_clean(struct DbDriver* const driver);

int db_drive_open_db(
    struct DbDriver* const driver,
    const char* db_path
);
void db_drive_close_db(struct DbDriver* const driver);

int db_drive_verify_key(
    struct DbDriver* const driver,
    struct MasterKey* const key
);
int db_drive_read_db_data(
    struct DbDriver* const driver,
    struct MasterKey* const key
);
