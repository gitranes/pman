#include "encrypt/raw/crypt_block.h"

#include <openssl/evp.h>

#include <stdlib.h>

static CryptBlock enc_get_block(const EVP_CIPHER* type, size_t input_size);

CryptBlock enc_crypt_block_init(enum EncryptAlgo algorithm, size_t input_size)
{
    const EVP_CIPHER* type = enc_algo_cipher_type(algorithm);
    if (!type)
    {
        return (CryptBlock){0};
    }
    return enc_get_block(type, input_size);
}

void enc_crypt_block_clean(CryptBlock block)
{
    free(block.buf);
}

static CryptBlock enc_get_block(const EVP_CIPHER* type, size_t input_size)
{
    const size_t block_size = EVP_CIPHER_block_size(type);
    return (CryptBlock){
        .buf = malloc(block_size + input_size),
        .size = block_size + input_size
    };
}
