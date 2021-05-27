#include "common/error_msg.h"

const char* const ERROR_AUTH_FMT =
    "Authentication failed for database '%s'";

const char* const ERROR_CONFIG_OPEN_FMT =
    "Could not open configuration file '%s'";
const char* const ERROR_CONFIG_READ_FMT =
    "Could not read configuration file '%s";

const char* const ERROR_CMD_NEW_EXISTING_PATH_FMT =
    "The specified database file already exists '%s'";
const char* const ERROR_MASTER_PW_VERIFY =
    "The master passwords are not the same.";

const char* const ERROR_INVALID_PATH =
    "The specified path is not valid '%s'";

const char* error_get_msg_for_cmd(enum CmdStatus status)
{
    switch (status)
    {
    case CMD_INVALID:
        return "Unrecognized command.";
    case CMD_BAD_AUTH:
        return "Authentication failed.";
    case CMD_BAD_DB_READ:
        return "Database could not be read";
    case CMD_NEW_BAD_DB_PATH:
        return "Database path was not valid.";
    case CMD_NEW_INIT_DB_FAILED:
        return "Database initialization failed.";
    case CMD_NO_DB:
        return "No database to modify.";
    case CMD_ADD_DB_UPDATE_FAIL:
        return "Updating database entries failed.";
    case CMD_GET_NOT_FOUND:
        return "No matching entry found.";
    default:
        return "Unrecognized error.";
    }
}
