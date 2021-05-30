#include "db/entries/entry.h"

#include "logging/logger.h"

#include <stdlib.h>
#include <string.h>

void db_entry_init_in_place(void* buffer)
{
    struct Entry* const entry = buffer;
    memset(entry, 0, sizeof(struct Entry));
}

void db_entry_clean_in_place(struct Entry* entry)
{
    free(entry->name);
    free(entry->user);
    free(entry->password);
}

void db_entry_set_name(struct Entry* entry, const char* name)
{
    entry->name = strdup(name);
}

void db_entry_set_user(struct Entry* entry, const char* user)
{
    entry->user = strdup(user);
}

void db_entry_set_password(struct Entry* entry, const char* password)
{
    entry->password = strdup(password);
}

size_t db_entry_strlen(const struct Entry* entry)
{
    if (!entry->name || !entry->user || !entry->password)
    {
        return 0;
    }
    return strlen(entry->name) + strlen(entry->user) + strlen(entry->password);
}

void db_entry_print(const struct Entry* entry, const struct Logger* logger)
{
    logger->interface.plain(logger,
        "Entry '%s': %s:%s", entry->name, entry->user, entry->password);
}
