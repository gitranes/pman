#include "encrypt/raw/crypt.h"

#include "encrypt/meta/crypt.h"

static int enc_decrypt(
    const struct CryptMeta* meta,
    CryptBlock* block,
    const EVP_CIPHER *type,
    const void* data, size_t size);

static int enc_encrypt(
    const struct CryptMeta* meta,
    CryptBlock* block,
    const EVP_CIPHER *type,
    const void* data, size_t size);

struct CryptSetup
{
    CryptBlock block;
    const EVP_CIPHER* type;
};

static struct CryptSetup enc_setup_crypt(
    enum EncryptAlgo algorithm, size_t size);

CryptBlock enc_decrypt_raw(
    const struct CryptMeta* meta, const void* data, size_t size)
{
    struct CryptSetup setup = enc_setup_crypt(meta->algorithm, size);

    if (enc_decrypt(meta, &setup.block, setup.type, data, size))
    {
        enc_crypt_block_clean(setup.block);
        return (CryptBlock){0};
    }
    return setup.block;
}

CryptBlock enc_encrypt_raw(
    const struct CryptMeta* meta, const void* data, size_t size)
{
    struct CryptSetup setup = enc_setup_crypt(meta->algorithm, size);

    if (enc_encrypt(meta, &setup.block, setup.type, data, size))
    {
        enc_crypt_block_clean(setup.block);
        return (CryptBlock){0};
    }
    return setup.block;
}

static struct CryptSetup enc_setup_crypt(
    enum EncryptAlgo algorithm, size_t size)
{
    struct CryptSetup setup = {0};
    setup.type = enc_algo_cipher_type(algorithm);
    if (!setup.type)
    {
        enc_crypt_block_clean(setup.block);
        return (struct CryptSetup){0};
    }

    setup.block = enc_crypt_block_init(algorithm, size);
    if (!setup.block.buf)
    {
        return (struct CryptSetup){0};
    }

    return setup;
}

static int enc_decrypt(
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
    if (EVP_DecryptInit(ctx, type, meta->key.buf, meta->iv.buf) != 1)
    {
        return -2;
    }

    int out_size = 0;
    if (EVP_DecryptUpdate(ctx, block->buf, &out_size, data, (int)size) != 1)
    {
        return -3;
    }
    block->size = out_size;

    if (EVP_DecryptFinal(ctx, block->buf + out_size, &out_size) != 1)
    {
        return -4;
    }
    block->size += out_size;

    EVP_CIPHER_CTX_free(ctx);
    return 0;
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
    if (EVP_EncryptInit(ctx, type, meta->key.buf, meta->iv.buf) != 1)
    {
        return -2;
    }

    int out_size = 0;
    if (EVP_EncryptUpdate(ctx, block->buf, &out_size, data, (int)size) != 1)
    {
        return -3;
    }
    block->size = out_size;

    if (EVP_EncryptFinal(ctx, block->buf + out_size, &out_size) != 1)
    {
        return -4;
    }
    block->size += out_size;

    EVP_CIPHER_CTX_free(ctx);
    return 0;
}
