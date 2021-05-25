#pragma once

enum CmdStatus
{
    CMD_OK = 0,
    CMD_INVALID,
    CMD_SETUP_FAIL,

    CMD_NEW_EXISTING_DB,
    CMD_NEW_BAD_DB_PATH,
    CMD_NEW_INIT_DB_FAILED,
    CMD_NEW_PROMPT_FAIL
};
