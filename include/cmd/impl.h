#pragma once

#include "cmd/cmd.h"
#include "cmd/error.h"

struct Options;

enum CmdNewError cmd_run_new(const struct Options* options);
enum CmdLoginError cmd_run_login(const struct Options* options);
enum CmdAddError cmd_run_add(const struct Options* options);
enum CmdGetError cmd_run_get(const struct Options* options);
enum CmdListError cmd_run_list(const struct Options* options);
enum CmdDelError cmd_run_del(const struct Options* options);
enum CmdEditError cmd_run_edit(const struct Options* options);

const char* cmd_get_error_msg(enum Command cmd, int error);
