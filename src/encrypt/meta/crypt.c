#include "encrypt/meta/crypt.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct CryptMeta* enc_crypt_meta_init(
    enum EncryptAlgo algorithm,
    struct ByteView key,
    struct ByteView iv)
{
    struct CryptMeta* crypt = malloc(sizeof(*crypt));
    crypt->algorithm = algorithm;

    const EVP_CIPHER* type = enc_algo_cipher_type(algorithm);
    assert(type);

    crypt->key.size = key.size;
    crypt->key.buf = malloc(key.size);
    memcpy(crypt->key.buf, key.buf, key.size);
    assert((size_t)EVP_CIPHER_key_length(type) == key.size);

    crypt->iv.size = iv.size;
    crypt->iv.buf = malloc(iv.size);
    memcpy(crypt->iv.buf, iv.buf, iv.size);
    assert((size_t)EVP_CIPHER_iv_length(type) == iv.size);

    return crypt;
}

void enc_crypt_meta_clean(struct CryptMeta* meta)
{
    free(meta->key.buf);
    free(meta->iv.buf);
    free(meta);
}
