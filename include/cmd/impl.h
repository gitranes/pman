#pragma once

#include "cmd/cmd.h"
#include "cmd/error.h"

struct Options;

enum CmdNewError cmd_run_new(struct Options* options);
enum CmdNewError cmd_run_login(struct Options* options);
enum CmdNewError cmd_run_add(struct Options* options);
enum CmdNewError cmd_run_get(struct Options* options);
enum CmdNewError cmd_run_list(struct Options* options);
enum CmdNewError cmd_run_del(struct Options* options);
enum CmdNewError cmd_run_edit(struct Options* options);

const char* cmd_get_error_msg(enum Command cmd, int error);
