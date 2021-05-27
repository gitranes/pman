#include "db/entries/entry.h"

#include "logging/logger.h"

#include <stdio.h>

void db_entry_print(const struct Entry* entry, const struct Logger* logger)
{
    logger->interface.plain(logger,
        "Entry '%s': %s:%s", entry->name, entry->user, entry->password);
}
