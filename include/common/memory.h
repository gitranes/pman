#pragma once

#include <stddef.h>

int mem_grow_buffer(void** buffer, size_t size, size_t new_size);
