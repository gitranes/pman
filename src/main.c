#include "cmd/setup.h"
#include "cmd/drive.h"

#include "common/error_msg.h"

#include "options/parse.h"

#include "logging/terminal_logger.h"

#include <stdlib.h>

int main(int argc, char* argv[])
{
    int exit_code = EXIT_SUCCESS;

    struct ParseResult* const parsed = options_parse_result_init();
    if (options_parse(parsed, argc, argv))
    {
        exit_code = EXIT_FAILURE;
        TLOG_ERROR("%s\n", error_get_msg_for_parse(parsed->status));
        goto parse_cleanup;
    }

    struct CmdRunEnvironment* const env = cmd_run_env_init(parsed->options);
    if (cmd_build_run_env(env))
    {
        exit_code = EXIT_FAILURE;
        TLOG_ERROR("%s\n", "Failed to configure running environment.");
        goto env_cleanup;
    }

    if (cmd_drive(env))
    {
        exit_code = EXIT_FAILURE;
        TLOG_ERROR("%s\n", "Command execution failed.");
        goto env_cleanup;
    }

env_cleanup:
    cmd_run_env_clean(env);
parse_cleanup:
    options_parse_result_clean(parsed);

    return exit_code;
}
