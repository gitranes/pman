#include "db/entries/category.h"

#include "common/memory.h"

#include <stdlib.h>
#include <string.h>

struct Category* db_category_init_in_place(void* buffer, const char* name)
{
    struct Category* const category = buffer;
    category->name = strdup(name);
    return category;
}

void db_category_clean_in_place(struct Category* category)
{
    free(category->name);
    for (size_t i = 0; i < category->entries.size; ++i)
    {
        db_entry_clean_in_place(&category->entries.buf[i]);
    }
}

struct Entry* db_category_find_entry(
    struct Category* category, const char* entry_name)
{
    for (size_t i = 0; category->entries.size; ++i)
    {
        struct Entry* const entry = &category->entries.buf[i];
        if (strcmp(entry->name, entry_name) == 0)
        {
            return entry;
        }
    }
    return NULL;
}

struct Entry* db_category_new_empty_entry(struct Category* category)
{
    const size_t old_size = category->entries.size;
    if (mem_grow_buffer(
        (void**)&category->entries.buf, old_size, old_size + 1))
    {
        return NULL;
    }
    category->entries.size += 1;

    struct Entry* const new_entry = &category->entries.buf[old_size];
    db_entry_init_in_place(new_entry);

    return new_entry;
}
