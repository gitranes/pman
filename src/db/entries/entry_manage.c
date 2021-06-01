#include "db/entries/entry_manage.h"

#include <common/macros.h>

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

enum FileSizes
{
    DB_LINE_SIZE = 256
};

enum TokenMap
{
    ENTRY_NAME = 0,
    ENTRY_USER,
    ENTRY_PW,
    ENTRY_END
};

static const char* const ENTRIES_CATEGORY_MARKER = "######";
static const char* const ENTRIES_DEFAULT_CATEGORY = "default";
static const char* const ENTRIES_SEPARATOR = ":";

static void db_entries_parse_line(
    struct DbEntries* entries,
    char* line,
    struct Category** current_category);

static int db_entries_set_line(
    char* line_buffer,
    size_t* slice_size,
    const unsigned char* line_start,
    size_t max_slice);

static void db_entries_parse_entry(
    struct Category* current_category, char* line);

static void db_entries_set_entry_data(
    struct Entry* entry, enum TokenMap token_pos, const char* token_value);

static size_t db_entries_raw_size(const struct DbEntries* entries);
static void db_entries_write_raw(
    const struct DbEntries* entries, struct ByteView* raw);


struct DbEntries* db_entries_init(const size_t category_count)
{
    // TODO: Support for more than 1
    assert(category_count <= 1);

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
        db_category_clean_in_place(&entries->categories.buf[i]);
    }
    free(entries->categories.buf);
    free(entries);
}

int db_entries_from_raw(struct DbEntries* entries, struct ByteView raw)
{
    // raw is not necessarily null terminated, so need to read in slices
    char line_buffer[DB_LINE_SIZE] = "";
    struct Category* current_category = NULL;

    for (size_t read_bytes = 0, slice_size = 0;
         read_bytes < raw.size;
         read_bytes += slice_size)
    {
        unsigned char* const line_start = raw.buf + read_bytes;

        // Choose a value that fits buffer and does not go over raw
        // Note: null byte
        const size_t max_slice =
            MIN(sizeof(line_buffer) - 1, raw.size - read_bytes - 1);

        if (db_entries_set_line(
                line_buffer, &slice_size, line_start, max_slice))
        {
            return -1;
        }
        db_entries_parse_line(entries, line_buffer, &current_category);
    }
    return 0;
}

struct ByteView db_entries_as_raw(const struct DbEntries* entries)
{
    const size_t raw_size = db_entries_raw_size(entries);
    struct ByteView raw = {
        .buf = malloc(raw_size),
        .size = raw_size
    };

    db_entries_write_raw(entries, &raw);
    return raw;
}

static size_t db_entries_raw_size(const struct DbEntries* entries)
{
    size_t raw_size = 0;
    const size_t cat_marker_size = strlen(ENTRIES_CATEGORY_MARKER);
    const size_t separator_size = strlen(ENTRIES_SEPARATOR);

    // begin and end category markers
    // Note: line break
    raw_size += entries->categories.size * 2 * (cat_marker_size + 1);

    for (size_t i = 0; i < entries->categories.size; ++i)
    {
        struct Category* const category = &entries->categories.buf[0];
        // ENTRY_END amount of separators per entry
        raw_size += ENTRY_END * (separator_size * category->entries.size);

        for (size_t j = 0; j < category->entries.size; ++j)
        {
            // Note: line break
            raw_size += db_entry_strlen(&category->entries.buf[j]) + 1;
        }
    }
    return raw_size;
}

static void db_entries_write_raw(
    const struct DbEntries* entries, struct ByteView* const raw)
{
    size_t written_bytes = 0;
    for (size_t i = 0; i < entries->categories.size; ++i)
    {
        struct Category* const category = &entries->categories.buf[0];
        written_bytes += snprintf(
            (char*)raw->buf + written_bytes,
            raw->size,
            "%s\n",
            ENTRIES_CATEGORY_MARKER);

        for (size_t j = 0; j < category->entries.size; ++j)
        {
            struct Entry* const entry = &category->entries.buf[j];
            written_bytes += snprintf(
                (char*)raw->buf + written_bytes,
                raw->size,
                "%s%s%s%s%s\n",
                entry->name,
                ENTRIES_SEPARATOR,
                entry->user,
                ENTRIES_SEPARATOR,
                entry->password);
        }
        written_bytes += snprintf(
            (char*)(*raw).buf + written_bytes,
            (*raw).size,
            "%s\n",
            ENTRIES_CATEGORY_MARKER);
    }
}

struct Category* db_entries_find_category(
    struct DbEntries* entries, const char* category_name)
{
    if (!category_name)
    {
        category_name = ENTRIES_DEFAULT_CATEGORY;
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

static int db_entries_set_line(
    char* line_buffer,
    size_t* const slice_size,
    const unsigned char* line_start,
    const size_t max_slice)
{
    unsigned char* const line_end = memchr(line_start, '\n', max_slice);
    if (!line_end)
    {
        return -1;
    }
    *slice_size = line_end - line_start;

    memcpy(line_buffer, line_start, *slice_size);
    line_buffer[*slice_size - 1] = '\0';

    return 0;
}

static void db_entries_parse_line(
    struct DbEntries* entries,
    char* const line,
    struct Category** current_category)
{
    if (!*current_category && strcmp(line, ENTRIES_CATEGORY_MARKER) == 0)
    {
        *current_category = db_category_init_in_place(
            &entries->categories.buf[0], ENTRIES_DEFAULT_CATEGORY);
        return;
    }
    if (*current_category && strcmp(line, ENTRIES_CATEGORY_MARKER) == 0)
    {
        // End of category
        *current_category = NULL;
        return;
    }
    db_entries_parse_entry(*current_category, line);
}

static void db_entries_parse_entry(
    struct Category* const current_category, char* const line)
{
    struct Entry* const entry = db_category_new_empty_entry(current_category);
    char* save_ptr = NULL;

    enum TokenMap token_pos = ENTRY_NAME;
    for (char* str1 = line;; str1 = NULL, ++token_pos)
    {
        char* const token = strtok_r(str1, ENTRIES_SEPARATOR, &save_ptr);
        if (!token)
        {
            assert(token_pos == ENTRY_END);
            return;
        }
        db_entries_set_entry_data(entry, token_pos, token);
    }
}

static void db_entries_set_entry_data(
    struct Entry* const entry, enum TokenMap token_pos, const char* token_value)
{
    switch (token_pos)
    {
    case ENTRY_NAME:
        db_entry_set_name(entry, token_value);
        break;
    case ENTRY_USER:
        db_entry_set_user(entry, token_value);
        break;
    case ENTRY_PW:
        db_entry_set_password(entry, token_value);
        break;
    default:
        assert(false);
    }
}
