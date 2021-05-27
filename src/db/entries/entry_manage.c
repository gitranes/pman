#include "db/entries/entry_manage.h"

#include <common/macros.h>

#include <stdlib.h>
#include <string.h>

const char* const CATEGORY_DEFAULT_NAME = "default";

struct DbEntries* db_entries_init(const size_t category_count)
{
    struct DbEntries* const entries = malloc(sizeof(*entries));
    entries->categories = (struct CategoryArray){
        .buf = calloc(category_count, sizeof(struct Category)),
        .size = category_count,
    };
    for (size_t i = 0; i < category_count; ++i)
    {
        db_category_init_in_place(&entries->categories.buf[i]);
    }

    return entries;
}

void db_entries_clean(struct DbEntries* const entries)
{
    for (size_t i = 0; i < entries->categories.size; i++)
    {
        db_category_clean_in_place(&entries->categories.buf[i]);
    }
    free(entries->categories.buf);
    free(entries);
}

struct Category* db_entries_find_category(
    struct DbEntries* entries, const char* category_name)
{
    if (!category_name)
    {
        category_name = CATEGORY_DEFAULT_NAME;
    }
    for (size_t i = 0; entries->categories.size; ++i)
    {
        struct Category* const category = &entries->categories.buf[i];
        if (strcmp(category->name, category_name) == 0)
        {
            return category;
        }
    }
    return NULL;
}

struct Entry* db_entries_find_entry(
    struct DbEntries* entries,
    const char* category_name,
    const char* entry_name)
{
    if (!category_name)
    {
        // Search all
        for (size_t i = 0; i < entries->categories.size; ++i)
        {
            struct Category* const category = &entries->categories.buf[i];
            struct Entry* const entry =
                db_category_find_entry(category, entry_name);
            if (entry)
            {
                return entry;
            }
        }
    }
    else
    {
        // Search the one category's entries
        struct Category* const category =
            db_entries_find_category(entries, category_name);
        if (category)
        {
            return db_category_find_entry(category, entry_name);
        }
    }
    return NULL;
}

void db_entries_list(
    struct DbEntries* entries,
    const struct Logger* logger,
    enum ListSetting setting)
{
    UNUSED(setting);
    // TODO: ListSetting

    for (size_t i = 0; i < entries->categories.size; ++i)
    {
        struct Category* const category = &entries->categories.buf[i];
        logger->interface.plain(logger, "Category '%s':\n", category->name);
        for (size_t j = 0; j < category->entries.size; ++j)
        {
            logger->interface.plain(logger, "\t");
            struct Entry* const entry = &category->entries.buf[j];
            db_entry_print(entry, logger);
        }
    }
}
