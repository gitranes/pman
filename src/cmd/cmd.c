#include "cmd/cmd.h"

#include <stddef.h>
#include <string.h>

enum CmdSizes
{
    CMD_AMOUNT = CMD_EDIT + 1,
    CMD_ALIASES_MAX = 4
};

// NULLs will signal end of the inner array
static const char* const CMD_STR_MAP[CMD_AMOUNT][CMD_ALIASES_MAX] = {
    [CMD_NO_CMD] = {NULL},
    [CMD_NEW] = {"new"},
    [CMD_LOGIN] = {"login"},
    [CMD_ADD] = {"add", "insert", "append"},
    [CMD_GET] = {"get", "fetch"},
    [CMD_LIST] = {"list", "ls"},
    [CMD_DEL] = {"del", "remove", "delete", "rm"},
    [CMD_EDIT] = {"edit"}
};

enum Command cmd_match_string(const char* str)
{
    for (size_t i = 0; i < CMD_AMOUNT; ++i)
    {
        for (size_t j = 0; j < CMD_AMOUNT && CMD_STR_MAP[i][j]; ++j)
        {
            if (strcmp(str, CMD_STR_MAP[i][j]) == 0)
            {
                return (enum Command)(i);
            }
        }
    }
    return CMD_NO_CMD;
}
