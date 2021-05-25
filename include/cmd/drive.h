#pragma once

struct CmdRunEnvironment;

enum CmdDriverStatus
{
    CMD_DRV_OK = 0,
    CMD_DRV_NO_CMD,
    CMD_DRV_FAIL
};

/**
 * Sets up the environment, recognizes, and runs the command (calls the matching
 * impl function)
 * @param options
 * @return see CmdDriverStatus
 */
enum CmdDriverStatus cmd_drive(const struct CmdRunEnvironment* env);
