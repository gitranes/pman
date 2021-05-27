#include "cmd/setup.h"

#include "db/driver.h"

#include "common/error_msg.h"
#include "common/paths.h"

#include "logging/terminal_logger.h"

#include "options/options.h"

#include "persistence/config.h"
#include "persistence/login_cache.h"

#include <stdlib.h>

/**
 * Tries to read the config file. Creates a new config file, if the existing
 * could not be opened.
 * @return Read config or NULL on failure
 */
static bool cmd_read_config();

/**
 * Opens the database if there is a valid path in config, otherwise
 * does nothing.
 * @return Opened database
 */
static int cmd_try_to_open_db(struct CmdRunEnvironment* env);

struct CmdRunEnvironment* cmd_run_env_init(const struct Options* options)
{
    struct CmdRunEnvironment* const env = malloc(sizeof(*env));
    env->options = options;
    env->db = db_drive_init();
    env->config = config_view_init();
    env->cache = login_cache_init();
    return env;
}

void cmd_run_env_clean(struct CmdRunEnvironment* env)
{
    // options is not owned by the environment
    config_view_clean(env->config);
    db_drive_clean(env->db);
    login_cache_clean(env->cache);
    free(env);
}

int cmd_build_run_env(struct CmdRunEnvironment* env)
{
    cmd_read_config(env->config);
    if (!env->config)
    {
        return -1;
    }
    cmd_try_to_open_db(env);
    // TODO: Login cache

    return 0;
}

static bool cmd_read_config(struct ConfigView* const config)
{
    if (config_open(config, PATH_CONFIG_FILE) && config_new(config, PATH_CONFIG_FILE))
    {
        TLOG_ERROR(ERROR_CONFIG_OPEN_FMT, PATH_CONFIG_FILE);
        return false;
    }
    if (config_read(config))
    {
        TLOG_ERROR(ERROR_CONFIG_READ_FMT, PATH_CONFIG_FILE);
        return false;
    }
    return true;
}

static int cmd_try_to_open_db(struct CmdRunEnvironment* env)
{
    // db_path from an option is prioritized
    struct OptionHolder* const path_opt =
        options_find(env->options, OPT_DB_PATH);
    if (path_opt
        && !db_drive_open_db(env->db, path_opt->args[0]))
    {
        return 0;
    }

    // Otherwise check the config
    if (env->config->active_db_path
        && !db_drive_open_db(env->db, env->config->active_db_path))
    {
        return 0;
    }

    return -1;
}
