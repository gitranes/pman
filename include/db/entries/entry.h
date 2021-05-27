#pragma once

struct Logger;

struct Entry
{
    const char* name;
    const char* user;
    const char* password;
};

void db_entry_init_in_place(void* buffer);
void db_entry_clean_in_place(struct Entry* entry);

void db_entry_set_name(struct Entry* entry, const char* name);
void db_entry_set_user(struct Entry* entry, const char* user);
void db_entry_set_password(struct Entry* entry, const char* password);

void db_entry_print(const struct Entry* entry, const struct Logger* logger);
