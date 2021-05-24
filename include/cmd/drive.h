#pragma once

struct Options;

enum DriverFault
{
    DRV_OK = 0,
    DRV_PARSE_FAIL,
    DRV_NO_COMMAND,
    DRV_CMD_FAIL
};

enum DriverFault cmd_drive(struct Options* options);
