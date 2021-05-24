#pragma once

#include "encrypt/algorithm.h"

#include "common/buffer_view.h"

#include <stddef.h>

typedef struct ByteView CryptBlock;

CryptBlock enc_init_encrypt_block(
    enum EncryptAlgo algorithm, size_t block_size);
CryptBlock enc_init_decrypt_block(
    enum EncryptAlgo algorithm, size_t block_size);

void enc_clean_block(CryptBlock block);
