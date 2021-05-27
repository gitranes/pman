#include "db/entries/entry_manage.h"

#include <common/macros.h>

#include <stdlib.h>

struct DbEntries* db_entries_init(const size_t category_count)
{
    struct DbEntries* const entries = malloc(sizeof(*entries));
    entries->categories = (struct CategoryArray){
        .buf = calloc(category_count, sizeof(struct Category)),
        .size = category_count,
    };
    return entries;
}

void db_entries_clean(struct DbEntries* const entries)
{
    for (size_t i = 0; i < entries->categories.size; i++)
    {
        db_category_destroy_in_place(&entries->categories.buf[i]);
    }
    free(entries->categories.buf);
    free(entries);
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
