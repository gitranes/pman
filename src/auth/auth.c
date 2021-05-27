#include "auth/auth.h"

#include "common/error_msg.h"

#include "encrypt/meta/key.h"

#include "db/driver.h"
#include "db/metadata.h"

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
    struct MasterKey* const empty_key,
    struct DbDriver* const db_driver,
    struct LoginCache* const cache)
{
    assert(db_driver->fp && db_driver->metadata);
    const struct StringView master_pass =
        auth_get_master_pass(db_driver->db_path, cache);

    if (!master_pass.buf)
    {
        // Prompt or verification failed
        return -1;
    }
    if (db_drive_verify_key(db_driver, empty_key, master_pass))
    {
        TLOG_ERROR(ERROR_AUTH_FMT, db_driver->db_path);
        return -2;
    }

    empty_key->verified = true;
    return 0;
}

struct MasterKey* auth_master_key_init(size_t size)
{
    struct MasterKey* const key = malloc(sizeof(struct MasterKey));
    key->view.buf = malloc(size);
    key->view.size = size;
    key->verified = false;

    return key;
}

void auth_master_key_clean(struct MasterKey* const key)
{
    free(key->view.buf);
    free(key);
}

struct StringView auth_get_master_pass(
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
