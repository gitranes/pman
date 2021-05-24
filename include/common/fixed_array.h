#pragma once

#include <stddef.h>

struct FixedArray
{
    void* buffer;
    size_t size;
    size_t obj_size;
};

struct StringArray
{
    char* buffer;
    size_t size;
};
