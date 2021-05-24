#pragma once

#include "encrypt/crypt_meta.h"
#include "encrypt/raw/crypt_block.h"

int enc_decrypt_raw_continous(
    struct CryptMeta meta,
    const void* data,
    size_t size,
    CryptBlock decrypt_block);

CryptBlock enc_decrypt_raw(
    struct CryptMeta meta, const void* data, size_t size);
