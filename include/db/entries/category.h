#pragma once

#include "db/entries/entry.h"

#include <stddef.h>

extern const char* const CATEGORY_DEFAULT_NAME;

struct EntryArray
{
    struct Entry* buf;
    size_t size;
};

struct Category
{
    char* name;
    struct EntryArray entries;
};

void db_category_init_in_place(void* buffer);
void db_category_clean_in_place(struct Category* category);

struct Entry* db_category_find_entry(
    struct Category* category, const char* entry_name);

struct Entry* db_category_new_empty_entry(struct Category* category);

void db_category_delete_entry(
    struct Category* category, struct Entry* entry);
