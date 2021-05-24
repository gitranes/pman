#pragma once

#include "hash/types.h"

struct Hasher
{
    void* context; // OpenSSL context for algorithm
    HashView view;
    enum HashAlgo algorithm;
};

struct Hasher* hash_hasher_init(enum HashAlgo algorithm);
void hash_hasher_clean(struct Hasher* hasher);

int hash_bytes_continous(
    struct Hasher* hasher,
    const void* data,
    size_t size);
