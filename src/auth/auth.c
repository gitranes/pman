#include "auth/auth.h"

#include "common/error_msg.h"

#include "db/driver.h"

#include "input/prompt_msgs.h"
#include "input/prompt_static.h"

#include "logging/terminal_logger.h"

#include "persistence/login_cache.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct StringView auth_password_prompt(const char* db_path);
static struct StringView auth_get_master_pass(
    const char* db_path, struct LoginCache* cache);

int auth_authenticate(
    struct DbDriver* const db_driver, struct LoginCache* const cache)
{
    assert(db_drive_db_is_open(db_driver));
    const struct StringView master_pass =
        auth_get_master_pass(db_driver->db_path, cache);

    if (!master_pass.buf)
    {
        // Prompt or verification failed
        return -1;
    }
    if (db_drive_unlock_db(db_driver, master_pass))
    {
        TLOG_ERROR(ERROR_AUTH_FMT, db_driver->db_path);
        return -2;
    }

    return 0;
}

static struct StringView auth_get_master_pass(
    const char* db_path, struct LoginCache* cache)
{
    struct StringView master_pass = {0};

    if (cache)
    {
        master_pass = login_cache_read(cache);
    }
    // cache failed or no cache
    if (!master_pass.buf)
    {
        master_pass = auth_password_prompt(db_path);
    }
    return master_pass;
}

static struct StringView auth_password_prompt(const char* db_path)
{
    const size_t prompt_size =
        strlen(PROMPT_AUTH_MASTER_PW_FMT) + strlen(db_path);
    char* const prompt_msg = malloc(prompt_size);

    snprintf(prompt_msg, prompt_size, PROMPT_AUTH_MASTER_PW_FMT, db_path);

    char* const master_password = prompt_static_password(prompt_msg);
    free(prompt_msg);

    return (struct StringView){
        .buf = master_password,
        .size = strlen(master_password)
    };
}
