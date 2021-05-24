#include "cmd/drive.h"
#include "cmd/impl.h"

#include "logging/terminal_logger.h"

#include "options/options.h"

static int run_command(const struct Options* options);

enum DriverFault cmd_drive(const struct Options* const options)
{
    const int cmd_run_status = run_command(options);
    if (!cmd_run_status)
    {
        return DRV_OK;
    }

    TLOG_ERROR("%s", cmd_get_error_msg(options->cmd, cmd_run_status));
    if (cmd_run_status == CMD_EMPTY)
    {
        return DRV_NO_COMMAND;
    }
    return DRV_CMD_FAIL;
}

static int run_command(const struct Options* const options)
{
    switch (options->cmd)
    {
    case CMD_UNRECOGNIZED:
        return CMD_UNRECOGNIZED;
    case CMD_EMPTY:
        return CMD_EMPTY;
    case CMD_NEW:
        return cmd_run_new(options);
    case CMD_LOGIN:
        return cmd_run_login(options);
    case CMD_ADD:
        return cmd_run_add(options);
    case CMD_DEL:
        return cmd_run_del(options);
    case CMD_EDIT:
        return cmd_run_edit(options);
    case CMD_GET:
        return cmd_run_get(options);
    case CMD_LIST:
        return cmd_run_list(options);
    default:
        return CMD_UNRECOGNIZED;
    }
}
