#pragma once

#include "hash/types.h"

HashView hash_bytes(const void* data, size_t size, enum HashAlgo algorithm);
