#include "db/driver.h"
#include "db/metadata.h"
#include <db/entries/entry_manage.h>
#include <db/master_key.h>

#include "common/file.h"

#include "encrypt/file/decrypt.h"
#include "encrypt/raw/crypt.h"

#include "hash/hash.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int db_drive_decrypt_data(struct DbDriver* driver);

static bool db_drive_verify_integrity_hash(const struct DbDriver* driver);
static int db_drive_set_integrity_hash(struct DbDriver* driver);

static HashView db_drive_calculate_integrity_hash(const struct DbDriver* driver);

static int db_drive_write_all(struct DbDriver* driver);

static int db_drive_write_metadata(struct DbDriver* driver);
static int db_drive_write_encrypted(struct DbDriver* driver);

static int db_drive_seek_to_meta_start(struct DbDriver* driver);
static int db_drive_seek_to_data_start(struct DbDriver* driver);

int db_drive_write_data(struct DbDriver* driver, CryptBlock encrypted);
CryptBlock db_drive_encrypt_data(const struct DbDriver* driver);

struct DbDriver* db_drive_init()
{
    struct DbDriver* const driver = malloc(sizeof(*driver));
    driver->fp = NULL;
    driver->metadata = db_meta_init();
    driver->entries = db_entries_init(1);
    driver->raw_data = (struct ByteView){ 0 };

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

int db_drive_new_db(
    struct DbDriver* driver,
    const char* db_path,
    struct StringView master_pass)
{
    if (!db_meta_new_db_values(driver->metadata))
    {
        return -1;
    }
    driver->fp = fopen(db_path, "w");
    if (!db_drive_db_is_open(driver))
    {
        return -2;
    }
    int exit_code = 0;
    struct MasterKey* const master_key = db_master_key_init();

    if (db_meta_calculate_key(driver->metadata, master_key, master_pass))
    {
        exit_code = -3;
        goto cleanup;
    }
    // Take ownership of the key
    driver->master_key = master_key;
    driver->master_key->verified = true;

    if (db_drive_write_all(driver))
    {
        exit_code = -4;
        goto cleanup;
    }

cleanup:
    db_master_key_clean(master_key);
    db_drive_close_db(driver);
    return exit_code;
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
    fclose(driver->fp);
    driver->fp = NULL;
}

int db_drive_unlock_db(struct DbDriver* driver, struct StringView master_pass)
{
    if (!db_drive_db_is_open(driver))
    {
        return -1;
    }
    driver->master_key = db_master_key_init();

    if (db_meta_calculate_key(driver->metadata, driver->master_key, master_pass))
    {
        return -2;
    }
    if (db_drive_decrypt_data(driver))
    {
        return -3;
    }
    if (!db_drive_verify_integrity_hash(driver))
    {
        return -4;
    }

    driver->master_key->verified = true;
    return 0;
}

int db_drive_read_db_data(struct DbDriver* driver)
{
    if (!driver->master_key->verified)
    {
        return -1;
    }
    if (!db_drive_db_is_open(driver))
    {
        return -2;
    }
    if (db_drive_decrypt_data(driver))
    {
        return -3;
    }
    if (db_entries_from_raw(driver->entries, driver->raw_data))
    {
        return -4;
    }
    return 0;
}

int db_drive_update_db_data(struct DbDriver* driver)
{
    if (!driver->master_key->verified)
    {
        return -1;
    }
    if (!db_drive_db_is_open(driver))
    {
        return -2;
    }
    if (freopen(NULL, "w", driver->fp))
    {
        return -3;
    }
    if (db_drive_write_all(driver))
    {
        db_drive_close_db(driver);
        return -4;
    }
    return 0;
}

static int db_drive_write_all(struct DbDriver* driver)
{
    driver->raw_data = db_entries_as_raw(driver->entries);
    if (!driver->raw_data.buf)
    {
        return -1;
    }
    if (db_drive_write_metadata(driver))
    {
        return -2;
    }
    if (db_drive_write_encrypted(driver))
    {
        return -3;
    }
    return 0;
}

static int db_drive_write_metadata(struct DbDriver* driver)
{
    if (db_drive_seek_to_meta_start(driver))
    {
        return -1;
    }
    if (db_drive_set_integrity_hash(driver))
    {
        return -2;
    }
    if (db_meta_write(driver->metadata, driver->fp))
    {
        return -3;
    }
    return 0;
}

int db_drive_write_encrypted(struct DbDriver* driver)
{
    const CryptBlock encrypted = db_drive_encrypt_data(driver);
    if (!encrypted.buf)
    {
        return -1;
    }
    if (db_drive_write_data(driver, encrypted))
    {
        enc_crypt_block_clean(encrypted);
        return -2;
    }

    enc_crypt_block_clean(encrypted);
    return 0;
}

CryptBlock db_drive_encrypt_data(const struct DbDriver* driver)
{
    assert(driver->master_key->verified);

    struct CryptMeta* const crypt_meta =
        db_meta_to_crypt_meta(driver->metadata, driver->master_key);
    if (!crypt_meta)
    {
        return (CryptBlock) { 0 };
    }
    const CryptBlock result = enc_encrypt_raw(
        crypt_meta, driver->raw_data.buf, driver->raw_data.size);

    enc_crypt_meta_clean(crypt_meta);
    return result;
}

int db_drive_write_data(struct DbDriver* driver, const CryptBlock encrypted)
{
    if (db_drive_seek_to_data_start(driver))
    {
        return -1;
    }
    fwrite(encrypted.buf, 1, encrypted.size, driver->fp);
    return 0;
}

static int db_drive_decrypt_data(struct DbDriver* driver)
{
    // Seek to the position of the data
    if (fseek(driver->fp, DB_METADATA_HEADER_SIZE, SEEK_SET))
    {
        return -1;
    }
    int ret_code = 0;
    struct CryptMeta* const crypt_meta =
        db_meta_to_crypt_meta(driver->metadata, driver->master_key);

    FILE* tmp_fp = NULL;
    // TODO: Something other than temp file?
    // Here a temp file is just as an "endless" buffer
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
    driver->raw_data = file_dump_contents(tmp_fp);

    enc_crypt_meta_clean(crypt_meta);
    fclose(tmp_fp);
    return 0;

error:
    enc_crypt_meta_clean(crypt_meta);
    return ret_code;
}

HashView db_drive_calculate_integrity_hash(const struct DbDriver* driver)
{
    const HashView hashed = hash_bytes(
        driver->raw_data.buf, driver->raw_data.size, HASH_SHA256);
    return hashed;
}

bool db_drive_verify_integrity_hash(const struct DbDriver* driver)
{
    const HashView hashed = db_drive_calculate_integrity_hash(driver);
    if (!hashed.buf)
    {
        return false;
    }
    const bool compare_result =
        memcmp(hashed.buf, driver->metadata->integrity_hash, hashed.size) == 0;

    free(hashed.buf);
    return compare_result;
}

static int db_drive_set_integrity_hash(struct DbDriver* driver)
{
    const HashView hashed = db_drive_calculate_integrity_hash(driver);
    if (!hashed.buf)
    {
        return -1;
    }
    assert(hashed.size == DB_INTEGRITY_HASH_SIZE);
    memcpy(driver->metadata->integrity_hash, hashed.buf, hashed.size);

    free(hashed.buf);
    return 0;
}

static int db_drive_seek_to_meta_start(struct DbDriver* driver)
{
    if (!db_drive_db_is_open(driver))
    {
        return -1;
    }
    return fseek(driver->fp, 0, SEEK_SET);
}

static int db_drive_seek_to_data_start(struct DbDriver* driver)
{
    if (!db_drive_db_is_open(driver))
    {
        return -1;
    }
    return fseek(driver->fp, DB_METADATA_HEADER_SIZE, SEEK_SET);
}
