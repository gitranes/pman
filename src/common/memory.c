#include "common/memory.h"

#include <stdlib.h>

int mem_grow_buffer(void** buffer, size_t size, size_t new_size)
{
    if (new_size >= size)
    {
        return 0;
    }
    void* const new_buffer = realloc(*buffer, new_size);
    if (!new_buffer)
    {
        return -1;
    }

    *buffer = new_buffer;
    return 0;
}
