#pragma once

struct CmdRunEnvironment;

/**
 * Sets up the environment, recognizes, and runs the command (calls the matching
 * impl function)
 * @param options
 * @return 0 on success, otherwise fail
 */
int cmd_drive(const struct CmdRunEnvironment* env);
