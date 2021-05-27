#pragma once

struct Options;
struct ConfigView;
struct DbDriver;
struct LoginCache;

struct CmdRunEnvironment
{
    const struct Options* options;
    struct ConfigView* config;
    struct DbDriver* db;
    struct LoginCache* cache;
};

struct CmdRunEnvironment* cmd_run_env_init(const struct Options* options);
void cmd_run_env_clean(struct CmdRunEnvironment* env);

/**
 * Sets up the environment (config and database) for all of the commands.
 * @return
 */
int cmd_build_run_env(struct CmdRunEnvironment* env);
