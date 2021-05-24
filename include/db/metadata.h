#pragma once

#include "encrypt/algorithm.h"

#include "common/buffer_view.h"

#include <stdio.h>

enum DbVersion
{
    DB_VER_UNRECOGNIZED = -1,
    DB_VER_1 = 1
};

struct DbMetadata
{
    enum EncryptAlgo encrypt_algo;
    enum DbVersion version;
    struct StringView master_seed;
    struct StringView encrypt_iv;
    size_t category_count;
    size_t entry_count;
    struct StringView integrity_hash;
    struct StringView transform_seed;
    size_t encrypt_rounds;
};

struct DbMetadata* db_meta_init();
void db_meta_clean(struct DbMetadata* meta);

int db_meta_read(struct DbMetadata* meta, FILE* fp_start);
