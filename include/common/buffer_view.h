// Simple sized views for different types of buffers. The user should use
// malloc/calloc/free for manipulating memory.
#pragma once

#include <stdalign.h>
#include <stddef.h>

struct BufferView
{
    void* buf;
    size_t size;
};

struct StringView
{
    char* buf;
    size_t size;
};

struct ByteView
{
    unsigned char* buf;
    size_t size;
};
