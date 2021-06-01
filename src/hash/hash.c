#include "hash/hash.h"

#include <openssl/evp.h>

#include <assert.h>

static HashView hash_digest(const EVP_MD *type, const void* data, size_t size);

const static HashView HASH_ERROR = {0};

HashView hash_bytes(const void* data, size_t size, enum HashAlgo algorithm)
{
    switch (algorithm)
    {
    case HASH_SHA256:
        return hash_digest(EVP_sha256(), data, size);
    case HASH_SHA512:
        return hash_digest(EVP_sha512(), data, size);
    default:
        return (HashView){0};
    }
}

static HashView hash_digest(const EVP_MD *type, const void* data, size_t size)
{
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (EVP_DigestInit_ex(context, type, NULL) != 1
        || EVP_DigestUpdate(context, data, size) != 1)
    {
        return HASH_ERROR;
    }
    const size_t block_size = EVP_MD_size(type);
    HashView hashed = {
        .buf = malloc(block_size),
        .size = block_size
    };

    unsigned digest_size = 0;
    if (EVP_DigestFinal_ex(context, hashed.buf, &digest_size) != 1)
    {
        free(hashed.buf);
        return (HashView){0};
    }
    assert(digest_size == hashed.size);

    EVP_MD_CTX_free(context);
    return hashed;
}
