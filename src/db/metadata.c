#include "db/metadata.h"

#include <openssl/rand.h>

#include <inttypes.h>
#include <stdlib.h>

// Previous end + size is the field's end offset
enum FieldEndOffsets
{
    DB_FLAGS_END = DB_FLAGS_SIZE,
    DB_MASTER_SEED_END = DB_FLAGS_END + DB_MASTER_SEED_SIZE,
    DB_ENCRYPT_IV_END = DB_MASTER_SEED_END + DB_ENCRYPT_IV_SIZE,
    DB_CATEGORY_COUNT_END = DB_ENCRYPT_IV_END + DB_CATEGORY_COUNT_SIZE,
    DB_ENTRY_COUNT_END = DB_CATEGORY_COUNT_END + DB_ENTRY_COUNT_SIZE,
    DB_CONTENT_HASH_END = DB_ENTRY_COUNT_END + DB_INTEGRITY_HASH_SIZE,
    DB_TRANSFORM_SEED_END = DB_CONTENT_HASH_END + DB_TRANSFORM_SEED_SIZE,
    DB_ENCRYPT_ROUND_END = DB_TRANSFORM_SEED_END + DB_ENCRYPT_ROUNDS_SIZE,
    DB_METADATA_SIZE = DB_ENCRYPT_ROUND_END
};

enum FieldCount
{
    DB_META_FIELD_COUNT = 8
};

// See enum FieldSizes and DbMetadata for explanation.
// SCNx64 = uint64_t from hexadecimal
static const char* const META_SCANF_STRING =
    "%" SCNx64 "%16s%16s%" SCNx64 "%" SCNx64 "%32s%32s%" SCNx64;

struct DbMetadata* db_meta_init()
{
    struct DbMetadata* const meta = calloc(1, sizeof(*meta));
    meta->version = DB_VER_UNRECOGNIZED;
    meta->encrypt_algo = ENC_ALGO_NONE;
    return meta;
}

void db_meta_clean(struct DbMetadata* meta)
{
    free(meta);
}

bool db_meta_new_db_values(struct DbMetadata* const meta)
{
    meta->version = (enum DbVersion)DB_DEFAULT_VERSION;
    meta->encrypt_algo = (enum EncryptAlgo)DB_DEFAULT_ENCRYPT_ALGO;
    meta->key_encrypt_rounds = DB_DEFAULT_KEY_ENCRYPT_ROUNDS;

    meta->category_count = DB_DEFAULT_CATEGORY_AMOUNT;
    meta->entry_count = 0;

    if (RAND_bytes(meta->master_seed, DB_MASTER_SEED_SIZE) != 1
        || RAND_bytes(meta->encrypt_iv, DB_MASTER_SEED_SIZE) != 1
        || RAND_bytes(meta->transform_seed, DB_MASTER_SEED_SIZE) != 1)
    {
        return false;
    }
    return true;
}

int db_meta_read(struct DbMetadata* meta, FILE* fp_start)
{
    static unsigned char meta_buffer[DB_METADATA_SIZE + 1] = "";

    if (fread(meta_buffer, 1, DB_METADATA_SIZE, fp_start)
        != DB_METADATA_SIZE)
    {
        return -1;
    }

    uint64_t flags = 0;

    // TODO: Better alternatives?
    if (sscanf((const char*)meta_buffer,
            META_SCANF_STRING,
            &flags,
            meta->master_seed,
            meta->encrypt_iv,
            &meta->category_count,
            &meta->entry_count,
            meta->integrity_hash,
            meta->transform_seed,
            &meta->key_encrypt_rounds)
        != DB_META_FIELD_COUNT)
    {
        return -2;
    }
    return 0;
}

int db_meta_calculate_key(
    struct DbMetadata* meta,
    struct MasterKey* empty_key,
    struct StringView master_pass)
{

}
