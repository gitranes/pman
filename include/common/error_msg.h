#pragma once

#include "cmd/status.h"

#include "options/parse.h"

extern const char* const ERROR_AUTH_FMT;
extern const char* const ERROR_PASSWORD_VERIFY;

extern const char* const ERROR_CONFIG_OPEN_FMT;
extern const char* const ERROR_CONFIG_READ_FMT;

extern const char* const ERROR_CMD_NEW_EXISTING_PATH_FMT;

extern const char* const ERROR_INVALID_PATH;

const char* error_get_msg_for_cmd(enum CmdStatus status);
const char* error_get_msg_for_parse(enum ParseStatus status);
