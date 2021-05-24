#pragma once

#include "encrypt/meta/crypt.h"
#include "encrypt/raw/crypt_block.h"

int enc_encrypt_raw_continous(
    struct CryptMeta meta,
    const void* data,
    size_t size,
    CryptBlock decrypt_block);

CryptBlock enc_encrypt_raw(
    struct CryptMeta meta, const void* data, size_t size);
