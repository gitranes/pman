#include "db/driver.h"
#include "db/metadata.h"
#include <db/entries/entry_manage.h>

#include "auth/auth.h"

#include "common/file.h"
#include "common/macros.h"

#include "encrypt/file/decrypt.h"

#include "hash/hash.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum DefaultSizes
{
    DB_DEFAULT_RAW_SIZE = 4096
};

static int db_decrypt_data(
    struct DbDriver* driver, const struct MasterKey* master_key);

static HashView db_calculate_integrity_hash(const struct DbDriver* driver);
static bool db_verify_integrity_hash(const struct DbDriver* driver);
__attribute__((unused)) static int db_set_integrity_hash(struct DbDriver* driver);

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
    return 0;
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
    if (!db_drive_db_is_open(driver))
    {
        return -1;
    }
    if (db_meta_calculate_key(driver->metadata, empty_key, master_pass))
    {
        return -2;
    }
    // MasterKey is now filled
    if (db_decrypt_data(driver, empty_key))
    {
        return -3;
    }
    if (!db_verify_integrity_hash(driver))
    {
        return -4;
    }
    return 0;
}

static int db_decrypt_data(
    struct DbDriver* driver, const struct MasterKey* master_key)
{
    // Seek to the position of the data
    if (fseek(driver->fp, DB_METADATA_HEADER_SIZE, SEEK_SET))
    {
        return -1;
    }
    int ret_code = 0;
    struct CryptMeta* const crypt_meta =
        db_meta_to_crypt_meta(driver->metadata, master_key);

    FILE* tmp_fp = NULL;
    // TODO: Something other than temp file?
    if (!(tmp_fp = file_hidden_temp(".", "w")))
    {
        ret_code = -2;
        goto error;
    }
    if (fenc_decrypt_to_file(crypt_meta, driver->fp, tmp_fp))
    {
        fclose(tmp_fp);
        ret_code = -3;
        goto error;
    }
    enc_crypt_meta_clean(crypt_meta);
    driver->raw_data = file_dump_contents(tmp_fp);
    fclose(tmp_fp);

    return 0;

error:
    enc_crypt_meta_clean(crypt_meta);
    return ret_code;
}

HashView db_calculate_integrity_hash(const struct DbDriver* driver)
{
    const HashView hashed = hash_bytes(
        driver->raw_data.buf, driver->raw_data.size, HASH_SHA256);
    return hashed;
}

bool db_verify_integrity_hash(const struct DbDriver* driver)
{
    const HashView hashed = db_calculate_integrity_hash(driver);
    if (!hashed.buf)
    {
        return false;
    }
    const bool compare_result =
        memcmp(hashed.buf, driver->metadata->integrity_hash, hashed.size) == 0;

    free(hashed.buf);
    return compare_result;
}

__attribute__((unused)) static int db_set_integrity_hash(struct DbDriver* driver)
{
    const HashView hashed = db_calculate_integrity_hash(driver);
    if (!hashed.buf)
    {
        return -1;
    }
    assert(hashed.size == DB_INTEGRITY_HASH_SIZE);
    memcpy(driver->metadata->integrity_hash, hashed.buf, hashed.size);

    free(hashed.buf);
    return 0;
}
