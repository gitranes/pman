#pragma once

#include "cmd/cmd.h"
#include "cmd/status.h"

struct CmdRunEnvironment;

enum CmdStatus cmd_run_new(const struct CmdRunEnvironment* env);
enum CmdStatus cmd_run_login(const struct CmdRunEnvironment* env);
enum CmdStatus cmd_run_add(const struct CmdRunEnvironment* env);
enum CmdStatus cmd_run_get(const struct CmdRunEnvironment* env);
enum CmdStatus cmd_run_list(const struct CmdRunEnvironment* env);
enum CmdStatus cmd_run_del(const struct CmdRunEnvironment* env);
enum CmdStatus cmd_run_edit(const struct CmdRunEnvironment* env);

const char* error_get_msg_for_cmd(enum CmdStatus status);
