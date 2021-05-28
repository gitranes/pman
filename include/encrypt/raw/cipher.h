#pragma once

#include <encrypt/raw/crypt_block.h>

#include <openssl/evp.h>

#include <stddef.h>

struct CryptMeta;

struct Cipher
{
    EVP_CIPHER_CTX* ctx;
    CryptBlock block;
};

enum CipherGoal
{
    ENC_CIPHER_DECRYPT = 0,
    ENC_CIPHER_ENCRYPT
};

struct Cipher* enc_cipher_init();
void enc_cipher_clean(struct Cipher* cipher);

int enc_cipher_prepare(
    struct Cipher* cipher,
    const struct CryptMeta* meta,
    size_t input_block_size,
    enum CipherGoal goal);
void enc_cipher_reset(struct Cipher* cipher);

int enc_cipher_run(struct Cipher* cipher, const void* data, size_t size);
