#include "cmd/impl.h"
#include "cmd/setup.h"

#include "auth/auth.h"

#include "common/constants.h"
#include "common/error_msg.h"
#include "common/macros.h"
#include "common/paths.h"

#include "db/driver.h"
#include "db/entries/entry_manage.h"
#include "db/master_key.h"
#include "db/metadata.h"

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
static struct StringView cmd_prompt_new_master_pw();

/**
 * Authenticates the user and reads the database's data
 * @param env
 * @return T/F
 */
static enum CmdStatus cmd_read_db(const struct CmdRunEnvironment* env);

/**
 * Adds a new entry to the database using the options and arguments user
 * had provided.
 * @param driver    - Driver with an authenticated and read database
 * @param options
 */
static void cmd_add_entry_to_db(
    struct DbDriver* driver, const struct Options* options);

static const char* cmd_add_prompt_user(const char* entry_name);
static const char* cmd_add_prompt_pw(const char* entry_name, bool echo_pw);

/**
 * Verifies that the path to the is a valid path (no strange chars) and that it
 * exists.
 * @param path - non-null path
 * @return T/F
 */
static bool cmd_db_path_verify(const char* path);

enum CmdStatus cmd_run_new(const struct CmdRunEnvironment* env)
{
    if (db_drive_db_is_open(env->db))
    {
        // Close possible recent db
        db_drive_close_db(env->db);
    }
    const char* new_db_path = env->options->argv[0];

    if (!cmd_db_path_verify(new_db_path))
    {
        return CMD_NEW_BAD_DB_PATH;
    }
    const struct StringView master_pw = cmd_prompt_new_master_pw();

    if (db_drive_new_db(env->db, new_db_path, master_pw))
    {
        return CMD_NEW_INIT_DB_FAILED;
    }

    return CMD_OK;
}

enum CmdStatus cmd_run_login(const struct CmdRunEnvironment* env)
{
    UNUSED(env);
    return CMD_OK;
}

enum CmdStatus cmd_run_add(const struct CmdRunEnvironment* env)
{
    enum CmdStatus read_status = CMD_OK;
    if ((read_status = cmd_read_db(env)) != CMD_OK)
    {
        return read_status;
    }

    cmd_add_entry_to_db(env->db, env->options);
    if (db_drive_update_db_data(env->db))
    {
        return CMD_ADD_DB_UPDATE_FAIL;
    }

    return CMD_OK;
}

enum CmdStatus cmd_run_get(const struct CmdRunEnvironment* env)
{
    enum CmdStatus read_status = CMD_OK;
    if ((read_status = cmd_read_db(env)) != CMD_OK)
    {
        return read_status;
    }
    const char* const entry_name = env->options->argv[0];
    struct Entry* const found_entry =
        db_entries_find_entry(env->db->entries, NULL, entry_name);

    if (!found_entry)
    {
        return CMD_GET_NOT_FOUND;
    }
    struct OptionHolder* clip_opt = options_find(env->options, OPT_CLIP);
    if (clip_opt)
    {
        // TODO: Clipboard management
    }

    // TODO: Other loggers
    db_entry_print(found_entry, &TERMINAL_LOGGER);
    return CMD_OK;
}

enum CmdStatus cmd_run_list(const struct CmdRunEnvironment* env)
{
    enum CmdStatus read_status = CMD_OK;
    if ((read_status = cmd_read_db(env)) != CMD_OK)
    {
        return read_status;
    }
    db_entries_list(env->db->entries, &TERMINAL_LOGGER, LIST_SHOW_USER_PW);
    return CMD_OK;
}

enum CmdStatus cmd_run_del(const struct CmdRunEnvironment* env)
{
    UNUSED(env);
    return CMD_OK;
}

enum CmdStatus cmd_run_edit(const struct CmdRunEnvironment* env)
{
    UNUSED(env);
    return CMD_OK;
}

static bool cmd_db_path_verify(const char* path)
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

static struct StringView cmd_prompt_new_master_pw()
{
    char* const master_pw = prompt_static_password_twice(
        PROMPT_NEW_DB_MASTER_PW, PROMPT_NEW_DB_VERIFY_MASTER_PW);

    return (struct StringView){
        .buf = master_pw,
        .size = strlen(master_pw)
    };
}

static enum CmdStatus cmd_read_db(const struct CmdRunEnvironment* env)
{
    if (!db_drive_db_is_open(env->db))
    {
        return CMD_NO_DB;
    }
    if (auth_authenticate(env->db, env->cache))
    {
        return CMD_BAD_AUTH;
    }
    if (db_drive_read_db_data(env->db))
    {
        return CMD_BAD_DB_READ;
    }
    return CMD_OK;
}

static const char* cmd_add_prompt_user(const char* entry_name)
{
    char* prompt_msg = prompt_fill_fmt(PROMPT_ADD_ENTRY_USERNAME_FMT, entry_name);

    const char* const user_input = prompt_static_text(prompt_msg);
    free(prompt_msg);
    return user_input;
}
static const char* cmd_add_prompt_pw(const char* entry_name, const bool echo_pw)
{
    char* const prompt_msg =
        prompt_fill_fmt(PROMPT_ADD_ENTRY_PASSWORD_FMT, entry_name);
    const char* pw_input = NULL;

    if (echo_pw)
    {
        pw_input = prompt_static_text(prompt_msg);
    }
    else
    {
        char* const verify_msg =
            prompt_fill_fmt(PROMPT_ADD_VERIFY_ENTRY_PASSWORD_FMT, entry_name);
        pw_input = prompt_static_password_twice(prompt_msg, verify_msg);
        free(verify_msg);
    }

    free(prompt_msg);
    return pw_input;
}

static void cmd_add_entry_to_db(
    struct DbDriver* driver, const struct Options* options)
{
    const char* const entry_name = options->argv[0];
    const char* category_name = NULL;
    bool echo_pw = false;

    struct OptionHolder* find_result = options_find(options, OPT_CATEGORY);
    if (find_result)
    {
        category_name = find_result->argv[0];
    }
    find_result = options_find(options, OPT_ECHO);
    if (find_result)
    {
        echo_pw = true;
    }
    struct Category* const category =
        db_entries_find_category(driver->entries, category_name);
    struct Entry* const new_entry = db_category_new_empty_entry(category);

    db_entry_set_name(new_entry, entry_name);
    db_entry_set_user(new_entry, cmd_add_prompt_user(new_entry->name));
    db_entry_set_password(new_entry, cmd_add_prompt_pw(new_entry->name, echo_pw));
}
