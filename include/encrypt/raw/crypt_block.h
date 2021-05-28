#pragma once

#include "encrypt/algorithm.h"

#include "common/buffer_view.h"

#include <stddef.h>

typedef struct ByteView CryptBlock;

CryptBlock enc_crypt_block_init(enum EncryptAlgo algorithm, size_t input_size);

void enc_crypt_block_clean(CryptBlock block);
