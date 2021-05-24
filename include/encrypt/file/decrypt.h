#pragma once

#include "encrypt/meta/crypt.h"

#include <stdio.h>

int fenc_decrypt_to_file(
    struct CryptMeta* meta, FILE* in_fp, FILE* out_fp);
FILE* fenc_decrypt_to_tmp(struct CryptMeta* meta, FILE* in_fp);
int fenc_decrypt_to_buffer(
    struct CryptMeta* meta, FILE* in_fp, void* buffer, size_t size);
