#pragma once

#include "common/buffer_view.h"

#include "encrypt/algorithm.h"
#include "encrypt/meta/crypt.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

struct MasterKey;

enum KeySizes
{
    DB_MASTER_KEY_SIZE = 64
};

enum FieldSizes
{
    DB_FLAGS_SIZE = 8,
    DB_MASTER_SALT_SIZE = 16,
    DB_ENCRYPT_IV_SIZE = 16,
    DB_CATEGORY_COUNT_SIZE = 8,
    DB_ENTRY_COUNT_SIZE = 8,
    DB_INTEGRITY_HASH_SIZE = 32,
    DB_ITERATION_ROUNDS_SIZE = 4
};

extern const long DB_METADATA_HEADER_SIZE;

enum DbVersion
{
    DB_VER_UNRECOGNIZED = 0,
    DB_VER_1 = 1
};

enum NewDefaults
{
    DB_DEFAULT_VERSION = DB_VER_1,
    DB_DEFAULT_ENCRYPT_ALGO = ENC_ALGO_AES_256,
    DB_DEFAULT_CATEGORY_AMOUNT = 1,
    DB_DEFAULT_KEY_ENCRYPT_ROUNDS = 10000
};

struct DbMetadata
{
    enum DbVersion version;
    enum EncryptAlgo encrypt_algo;
    unsigned char master_salt[DB_MASTER_SALT_SIZE + 1];
    unsigned char encrypt_iv[DB_ENCRYPT_IV_SIZE + 1];
    uint64_t category_count;
    uint64_t entry_count;
    unsigned char integrity_hash[DB_INTEGRITY_HASH_SIZE + 1];
    int32_t key_iteration_rounds;
};

struct DbMetadata* db_meta_init();
void db_meta_clean(struct DbMetadata* meta);

/**
 * Initializes the different values of DbMetadata for use with a new database.
 * NewDefaults is used along with a CSPRNG for generating the seeds and iv.
 * Only integrity_hash is left uninitialized.
 * @param meta
 */
bool db_meta_new_db_values(struct DbMetadata* meta);

/**
 * Attempts to read a metadata header from a file.
 * @param meta
 * @param fp_start
 * @return 0 on success and non-zero on failure
 */
int db_meta_read(struct DbMetadata* meta, FILE* fp_start);

/**
 * Calculates the master key based on the metadata and the given master password.
 * @param meta
 * @param empty_key
 * @param master_pass
 * @return 0 on success, non-zero on fail
 */
int db_meta_calculate_key(
    struct DbMetadata* meta,
    struct MasterKey* empty_key,
    struct StringView master_pass);

/**
 * Create CryptMeta object from the metadata stored in the metadata obj.
 * @param master_key
 * @return CryptMeta object (use enc_crypt_meta_clean when done)
 */
struct CryptMeta* db_meta_to_crypt_meta(
    const struct DbMetadata* meta, const struct MasterKey* master_key);
