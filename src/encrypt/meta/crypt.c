#include "encrypt/meta/crypt.h"

#include <stdlib.h>
#include <string.h>

struct CryptMeta* enc_crypt_meta_init(
    enum EncryptAlgo algorithm,
    struct ByteView key,
    struct ByteView iv)
{
    struct CryptMeta* crypt = malloc(sizeof(*crypt));
    crypt->algorithm = algorithm;

    crypt->key.size = key.size;
    crypt->key.buf = malloc(key.size);
    memcpy(crypt->key.buf, key.buf, key.size);

    crypt->iv.size = iv.size;
    crypt->iv.buf = malloc(iv.size);
    memcpy(crypt->iv.buf, iv.buf, iv.size);

    return crypt;
}

void enc_crypt_meta_clean(struct CryptMeta* meta)
{
    free(meta->key.buf);
    free(meta->iv.buf);
    free(meta);
}
