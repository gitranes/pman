#pragma once

#include "encrypt/algorithm.h"

#include "common/buffer_view.h"

#include <stddef.h>

struct CryptMeta
{
    struct ByteView init_vector;
    struct ByteView key;
    enum EncryptAlgo algorithm;
};

struct CryptMeta* enc_crypt_meta_init(
    enum EncryptAlgo algorithm,
    struct ByteView key,
    struct ByteView iv);
void enc_crypt_meta_clean(struct CryptMeta* meta);
