#include "db/master_key.h"
#include "db/metadata.h"

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>

// Previous end + size is the field's end offset
enum FieldEndOffsets
{
    DB_FLAGS_END = DB_FLAGS_SIZE,
    DB_MASTER_SEED_END = DB_FLAGS_END + DB_MASTER_SALT_SIZE,
    DB_ENCRYPT_IV_END = DB_MASTER_SEED_END + DB_ENCRYPT_IV_SIZE,
    DB_CATEGORY_COUNT_END = DB_ENCRYPT_IV_END + DB_CATEGORY_COUNT_SIZE,
    DB_ENTRY_COUNT_END = DB_CATEGORY_COUNT_END + DB_ENTRY_COUNT_SIZE,
    DB_CONTENT_HASH_END = DB_ENTRY_COUNT_END + DB_INTEGRITY_HASH_SIZE,
    DB_ENCRYPT_ROUND_END = DB_CONTENT_HASH_END + DB_ITERATION_ROUNDS_SIZE,
    DB_METADATA_SIZE = DB_ENCRYPT_ROUND_END
};

const long DB_METADATA_HEADER_SIZE = DB_METADATA_SIZE;

enum MetaHelp
{
    DB_META_FIELD_COUNT = 7,
    DB_META_FLAGS_SHIFT = 61
};

// See enum FieldSizes and DbMetadata for explanation.
// SCNx64 = uint64_t from hexadecimal
static const char* const METADATA_SCANF_STR =
    "%" SCNx64 "%16c%" SCNx64 "%16c%" SCNx64 "%32c%" SCNx32;

static const char* const METADATA_PRINTF_STR =
    "%" SCNx64 "%16s%" SCNx64 "%16s%" SCNx64 "%32s%" SCNx32;

static uint64_t db_meta_to_flags(const struct DbMetadata* meta);
static void db_meta_from_flags(struct DbMetadata* meta, uint64_t flags);

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
    meta->key_iteration_rounds = DB_DEFAULT_KEY_ENCRYPT_ROUNDS;

    meta->category_count = DB_DEFAULT_CATEGORY_AMOUNT;
    meta->entry_count = 0;

    if (RAND_bytes(meta->master_salt, DB_MASTER_SALT_SIZE) != 1
        || RAND_bytes(meta->encrypt_iv, DB_MASTER_SALT_SIZE) != 1)
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
    const int fields_assigned = sscanf(
        (const char*)meta_buffer,
        METADATA_SCANF_STR,
        &flags,
        meta->master_salt,
        &meta->category_count,
        meta->encrypt_iv,
        &meta->entry_count,
        meta->integrity_hash,
        &meta->key_iteration_rounds);
    if (fields_assigned != DB_META_FIELD_COUNT)
    {
        return -2;
    }
    db_meta_from_flags(meta, flags);
    return 0;
}


int db_meta_write(struct DbMetadata* meta, FILE* fp_start)
{
    const uint64_t flags = db_meta_to_flags(meta);

    const int bytes_written = fprintf(
        fp_start,
        METADATA_PRINTF_STR,
        flags,
        meta->master_salt,
        meta->category_count,
        meta->encrypt_iv,
        meta->entry_count,
        meta->integrity_hash,
        meta->key_iteration_rounds);

    return bytes_written > 0 ? 0 : -1;
}

int db_meta_calculate_key(
    struct DbMetadata* meta,
    struct MasterKey* empty_key,
    struct StringView master_pass)
{
    if (PKCS5_PBKDF2_HMAC_SHA1(
            master_pass.buf,
            (int)master_pass.size,
            meta->master_salt,
            DB_MASTER_SALT_SIZE,
            meta->key_iteration_rounds,
            (int)empty_key->view.size,
            empty_key->view.buf)
        != 1)
    {
        return -1;
    }
    return 0;
}

struct CryptMeta* db_meta_to_crypt_meta(
    const struct DbMetadata* meta, const struct MasterKey* master_key)
{
    // TODO: ugly const_cast
    return enc_crypt_meta_init(
        meta->encrypt_algo,
        master_key->view,
        (struct ByteView){
            .buf = (unsigned char*)meta->encrypt_iv,
            .size = DB_ENCRYPT_IV_SIZE
        }
    );
}

static uint64_t db_meta_to_flags(const struct DbMetadata* meta)
{
    uint64_t flags = meta->version;
    flags |= ((uint64_t)meta->encrypt_algo << (uint64_t)DB_META_FLAGS_SHIFT);
    return flags;
}

static void db_meta_from_flags(struct DbMetadata* meta, uint64_t flags)
{
    meta->encrypt_algo = (flags >> (uint64_t)DB_META_FLAGS_SHIFT);
    meta->version = (int32_t)flags;
}
