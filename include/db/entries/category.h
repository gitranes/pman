#pragma once

#include "db/entries/entry.h"

#include "common/fixed_array.h"

struct Category
{
    char* name;
    struct FixedArray entries; // array of Entry(s)
};

struct Category* db_category_init_in_place(void* buffer);
void db_category_destroy_in_place(struct Category* category);

struct Entry* db_category_find_entry(
    struct Category* category, const char* entry_name);

void db_category_delete_entry(
    struct Category* category, struct Entry* entry);
