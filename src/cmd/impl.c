#include "cmd/impl.h"
#include "cmd/setup.h"

#include "common/constants.h"
#include "common/error_msg.h"
#include "common/paths.h"

#include "db/driver.h"

#include "logging/terminal_logger.h"

#include "options/options.h"

#include "input/prompt_msgs.h"
#include "input/prompt_static.h"

#include <stdlib.h>
#include <string.h>

/**
 * Fetch a new verified master password to a static buffer
 * @return view to the buffer
 */
static struct StringView prompt_new_master_pw();

static bool db_path_verify(const char* path);

enum CmdStatus cmd_run_new(const struct CmdRunEnvironment* env)
{
    if (env->db)
    {
        // Close possible recent db
        db_drive_close_db(env->db);
    }
    const char* new_db_path = env->options->args[0];

    if (!db_path_verify(new_db_path))
    {
        return CMD_NEW_BAD_DB_PATH;
    }
    const struct StringView master_pw = prompt_new_master_pw();

    if (db_drive_new_db(env->db, new_db_path, master_pw))
    {
        return CMD_NEW_INIT_DB_FAILED;
    }

    return CMD_OK;
}

enum CmdStatus cmd_run_login(const struct CmdRunEnvironment* env)
{
    return CMD_OK;
}

enum CmdStatus cmd_run_add(const struct CmdRunEnvironment* env)
{
    return CMD_OK;
}

enum CmdStatus cmd_run_get(const struct CmdRunEnvironment* env)
{
    return CMD_OK;
}

enum CmdStatus cmd_run_list(const struct CmdRunEnvironment* env)
{
    return CMD_OK;
}

enum CmdStatus cmd_run_del(const struct CmdRunEnvironment* env)
{
    return CMD_OK;
}

enum CmdStatus cmd_run_edit(const struct CmdRunEnvironment* env)
{
    return CMD_OK;
}

const char* cmd_get_error_msg(enum CmdStatus status)
{
    return "ERROR";
}

static bool db_path_verify(const char* path)
{
    if (!path_valid_path(path))
    {
        TLOG_ERROR(ERROR_INVALID_PATH, path);
        return false;
    }
    if (path_exists(path))
    {
        TLOG_ERROR(ERROR_CMD_NEW_EXISTING_PATH_FMT, path);
        return false;
    }
    return true;
}

static struct StringView prompt_new_master_pw()
{
    static char verify_buffer[CST_MAX_MASTER_PW_LEN + 1] = "";
    struct StringView input = {
        .buf = verify_buffer,
        .size = 0
    };

    // Prompt master password twice and compare
    bool verify_ok = false;
    while (!verify_ok)
    {
        const char* input_pw = prompt_static_password(PROMPT_NEW_DB_MASTER_PW);
        input.size = strlen(input_pw);
        memcpy(input.buf, input_pw, input.size);

        input_pw = prompt_static_password(PROMPT_NEW_DB_VERIFY_MASTER_PW);
        if (strcmp(input.buf, input_pw) == 0)
        {
            verify_ok = true;
        }
        else
        {
            TLOG_ERROR("%s", ERROR_MASTER_PW_VERIFY);
        }
    }
    return input;
}
