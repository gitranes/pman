#pragma once

enum CmdStatus
{
    CMD_OK = 0,
    CMD_INVALID,
    CMD_SETUP_FAIL,
    CMD_BAD_AUTH,
    CMD_BAD_DB_READ,

    CMD_NEW_EXISTING_DB,
    CMD_NEW_BAD_DB_PATH,
    CMD_NEW_INIT_DB_FAILED,
    CMD_NEW_PROMPT_FAIL,

    CMD_ADD_NO_DB,
    CMD_ADD_DB_UPDATE_FAIL
};
