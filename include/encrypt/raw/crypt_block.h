#pragma once

#include "encrypt/algorithm.h"

#include "common/buffer_view.h"

#include <stddef.h>

typedef struct ByteView CryptBlock;

CryptBlock enc_encrypt_block_init(
    enum EncryptAlgo algorithm, size_t block_size);
CryptBlock enc_decrypt_block_init(
    enum EncryptAlgo algorithm, size_t block_size);

void enc_crypt_block_clean(CryptBlock block);
