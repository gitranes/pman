#pragma once

#include "encrypt/raw/crypt_block.h"

struct CryptMeta;

CryptBlock enc_decrypt_raw(
    const struct CryptMeta* meta, const void* data, size_t size);

CryptBlock enc_encrypt_raw(
    const struct CryptMeta* meta, const void* data, size_t size);
