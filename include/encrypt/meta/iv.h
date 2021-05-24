#pragma once

#include "encrypt/algorithm.h"

#include "common/buffer_view.h"

#include <stddef.h>

struct ByteView enc_fill_iv(enum EncryptAlgo algorithm);
size_t enc_get_iv_size(enum EncryptAlgo algorithm);
