#include "common/error_msg.h"

const char* const ERROR_AUTH_FMT =
    "Authentication failed for database '%s'.\n";
const char* const ERROR_PASSWORD_VERIFY =
    "The passwords are not the same. Try again.";

const char* const ERROR_CONFIG_OPEN_FMT =
    "Could not open configuration file '%s'.\n";
const char* const ERROR_CONFIG_READ_FMT =
    "Could not read configuration file '%s'.\n";

const char* const ERROR_CMD_NEW_EXISTING_PATH_FMT =
    "The specified database file '%s' already exists.\n";

const char* const ERROR_INVALID_PATH =
    "The specified path is not valid '%s'.\n";

const char* error_get_msg_for_cmd(enum CmdStatus status)
{
    switch (status)
    {
    case CMD_BAD_AUTH:
        return "Authentication failed.";
    case CMD_BAD_DB_READ:
        return "Database could not be read";
    case CMD_NEW_BAD_DB_PATH:
        return "Database path was not valid.";
    case CMD_NEW_INIT_DB_FAILED:
        return "Database initialization failed.";
    case CMD_NEW_PROMPT_FAIL:
        return "Master password prompt failed.";
    case CMD_NO_DB:
        return "No database to work with.";
    case CMD_ADD_DB_UPDATE_FAIL:
        return "Updating database entries failed.";
    case CMD_GET_NOT_FOUND:
        return "No matching entry found.";
    default:
        return "Unrecognized error.";
    }
}

const char* error_get_msg_for_parse(enum ParseStatus status)
{
    switch (status)
    {
    case PARSE_NO_CMD:
        return "No command specified.";
    case PARSE_UNRECOGNIZED_OPTION:
        return "Unrecognized option.";
    case PARSE_MISSING_ARGUMENT:
        return "Command is missing an argument.";
    case PARSE_UNSUPPORTED_CMD:
        return "Unsupported command.";
    case PARSE_UNSUPPORTED_OPT:
        return "Unsupported option.";
    default:
        return "Unrecognized parsing error.";
    }
}
