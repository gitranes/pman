#pragma once

enum Command
{
    CMD_NO_CMD = 0,
    CMD_NEW,
    CMD_LOGIN,
    CMD_ADD,
    CMD_GET,
    CMD_LIST,
    CMD_DEL,
    CMD_EDIT
};

enum CommandRequiredArgs
{
    CMD_NEW_ARG_REQUIRED = 1,
    CMD_LOGIN_ARG_REQUIRED = 1,
    CMD_ADD_ARG_REQUIRED = 1,
    CMD_GET_ARG_REQUIRED = 1,
    CMD_LIST_ARG_REQUIRED = 0,
    CMD_DEL_ARG_REQUIRED = 1,
    CMD_EDIT_ARG_REQUIRED = 1,
};

enum Command cmd_match_string(const char* str);
