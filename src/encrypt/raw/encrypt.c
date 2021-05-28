#include "encrypt/raw/encrypt.h"

#include "encrypt/raw/crypt_block.h"

#include <openssl/aes.h>
#include <openssl/evp.h>

static int enc_encrypt(
    const struct CryptMeta* meta,
    CryptBlock* block,
    const EVP_CIPHER *type,
    const void* data, size_t size);

CryptBlock enc_encrypt_raw(
    const struct CryptMeta* meta, const void* data, size_t size)
{
    CryptBlock block = enc_crypt_block_init(meta->algorithm, size);
    if (!block.buf)
    {
        return block;
    }
    const EVP_CIPHER* type = enc_algo_cipher_type(meta->algorithm);
    if (!type)
    {
        return (CryptBlock){0};
    }
    if (enc_encrypt(meta, &block, type, data, size))
    {
        return (CryptBlock){0};
    }
    return block;
}

static int enc_encrypt(
    const struct CryptMeta* meta,
    CryptBlock* const block,
    const EVP_CIPHER *type,
    const void* data, size_t size)
{
    EVP_CIPHER_CTX* const ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        return -1;
    }
    if (EVP_EncryptInit(ctx, type, meta->key.buf, meta->init_vector.buf) != 1)
    {
        return -2;
    }

    int out_size = 0;
    if ((EVP_EncryptUpdate(ctx, block->buf, &out_size, data, (int)size) != 1)
        || EVP_EncryptFinal(ctx, block->buf, &out_size) != 1)
    {
        return -3;
    }
    block->size = out_size;
    return 0;
}
