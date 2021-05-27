#include "common/memory.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int mem_grow_buffer(void** buffer, size_t size, size_t new_size)
{
    if (new_size >= size)
    {
        return 0;
    }
    void* const new_buffer = malloc(new_size);
    if (!new_buffer)
    {
        return -1;
    }

    memcpy(new_buffer, *buffer, size);
    *buffer = new_buffer;
    return 0;
}
