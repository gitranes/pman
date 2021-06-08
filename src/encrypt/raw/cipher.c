#include "encrypt/raw/cipher.h"

#include <encrypt/algorithm.h>
#include <encrypt/meta/crypt.h>

#include <assert.h>

struct Cipher* enc_cipher_init()
{
    struct Cipher* const cipher = malloc(sizeof(*cipher));
    cipher->ctx = EVP_CIPHER_CTX_new();
    cipher->block = (CryptBlock){0};
    return cipher;
}

void enc_cipher_clean(struct Cipher* cipher)
{
    EVP_CIPHER_CTX_free(cipher->ctx);
    enc_crypt_block_clean(cipher->block);
    free(cipher);
}

int enc_cipher_prepare(
    struct Cipher* cipher,
    const struct CryptMeta* meta,
    const size_t input_block_size,
    enum CipherGoal goal)
{
    static_assert(ENC_CIPHER_DECRYPT == 0, "OpenSSL translates 0 as decrypt");
    static_assert(ENC_CIPHER_ENCRYPT == 1, "OpenSSL translates 1 as encrypt");

    const EVP_CIPHER* type = enc_algo_cipher_type(meta->algorithm);
    if (!type)
    {
        return -1;
    }
    cipher->block = enc_crypt_block_init(meta->algorithm, input_block_size);
    if (!cipher->block.buf)
    {
        return -2;
    }
    if (EVP_CipherInit(cipher->ctx, type, meta->key.buf, meta->iv.buf, goal) != 1)
    {
        return -3;
    }
    return 0;
}

void enc_cipher_reset(struct Cipher* cipher)
{
    EVP_CIPHER_CTX_reset(cipher->ctx);
}

int enc_cipher_run(struct Cipher* cipher, const void* data, size_t size)
{
    assert(cipher->block.size > size);

    int out_bytes = 0;
    if (EVP_CipherUpdate(
            cipher->ctx, cipher->block.buf, &out_bytes, data, (int)size) != 1)
    {
        return -1;
    }
    cipher->block.size = out_bytes;

    if (EVP_CipherFinal(
            cipher->ctx, cipher->block.buf + out_bytes, &out_bytes) != 1)
    {
        return -2;
    }
    cipher->block.size += out_bytes;
    return 0;
}
