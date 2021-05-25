#include "cmd/drive.h"
#include "cmd/impl.h"
#include "cmd/setup.h"

#include "logging/terminal_logger.h"

#include "options/options.h"

#include <assert.h>

/**
 * Matches and runs the command in the environment
 * @param options - Options with a valid command (recognized and non-empty)
 * @return see CmdStatus
 */
static enum CmdStatus run_command(const struct CmdRunEnvironment* env);

enum CmdDriverStatus cmd_drive(const struct CmdRunEnvironment* env)
{
    const struct Options* opt = env->options;

    // Empty and unrecognized command should be handled during parsing
    assert(opt->cmd >= CMD_NEW && opt->cmd <= CMD_EDIT);

    const enum CmdStatus status = run_command(env);
    if (!status)
    {
        return CMD_DRV_OK;
    }

    TLOG_ERROR("%s", cmd_get_error_msg(status));
    if (status == CMD_INVALID)
    {
        return CMD_DRV_NO_CMD;
    }
    return CMD_DRV_FAIL;
}

static enum CmdStatus run_command(const struct CmdRunEnvironment* env)
{
    switch (env->options->cmd)
    {
    case CMD_NEW:
        return cmd_run_new(env);
    case CMD_LOGIN:
        return cmd_run_login(env);
    case CMD_ADD:
        return cmd_run_add(env);
    case CMD_DEL:
        return cmd_run_del(env);
    case CMD_EDIT:
        return cmd_run_edit(env);
    case CMD_GET:
        return cmd_run_get(env);
    case CMD_LIST:
        return cmd_run_list(env);
    default:
        return CMD_INVALID;
    }
}
