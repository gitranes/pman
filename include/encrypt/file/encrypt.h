#pragma once

#include "encrypt/meta/crypt.h"

#include <stdio.h>

int fenc_encrypt_to_file(
    struct CryptMeta* meta, FILE* in_fp, FILE* out_fp);
