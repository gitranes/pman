#pragma once

#include "db/entries/category.h"
#include "db/entries/entry.h"

#include "logging/logger.h"

#include <stddef.h>
#include <stdio.h>

struct CategoryArray
{
    struct Category* buf;
    size_t size;
};

struct DbEntries
{
    struct CategoryArray categories;
};

struct DbEntries* db_entries_init(size_t category_count);
void db_entries_clean();

int db_entries_read_db(struct DbEntries* entries, FILE* category_start);

struct StringView db_entries_write_to_buffer(struct DbEntries* entries);
int db_entries_write_to_file(struct DbEntries* entries, FILE* fp);

struct Category* db_entries_find_category(
    struct DbEntries* entries, const char* category_name);

/**
 * Searches the categories in the database for an entry
 * @param entries
 * @param category_name - Optional category_name (every category searched if NULL)
 * @param entry_name    - The name for the entry
 * @return ptr to Entry object
 */
struct Entry* db_entries_find_entry(
    struct DbEntries* entries,
    const char* category_name,
    const char* entry_name);

enum ListSetting
{
    LIST_HIDE_USER_PW = 0,
    LIST_SHOW_USER,
    LIST_SHOW_USER_PW
};

void db_entries_list(
    struct DbEntries* entries,
    const struct Logger* logger,
    enum ListSetting setting);
