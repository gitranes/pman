#include "options/parse.h"

#include <argp.h>

#include <stdbool.h>
#include <stdlib.h>

static error_t parse_opt(int key, char* arg, struct argp_state* state);
static bool parse_check_command_support(enum Command cmd);
static bool parse_check_cmd_arg_count(enum Command cmd, unsigned int end_arg_num);

static error_t parse_arg(
    char* arg,
    const struct argp_state* state,
    struct ParseResult* result);

// agrp string for --version
const char* argp_program_version = "pman 0.1.0"; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables, readability-identifier-naming)

static const char* const DOC = "pman -- a command line password manager.";
static const char* const ARG_DOC = "COMMAND [ARGS...]";

static const struct argp_option OPTIONS[] = {
    {
        .name = "echo",
        .key = 'e',
        .arg = NULL,
        .flags = 0,
        .group = 0,
        .doc = "Enables echoing with 'get' command.",
    },
    {
        .name = "db-path", .key = 'd', .arg = "db_path",
        .flags = 0, .group = 0,
        .doc = "Optional path to a database (defaults to recently used).",
    },
    {
        .name = "category", .key = 'c', .arg = "category",
        .flags = 0, .group = 0,
        .doc = "Category name for 'get' command.",
    },
    {
        .name = "clip", .key = 'x', .arg = NULL,
        .flags = 0, .group = 0,
        .doc = "Copies the password to the clipboard with 'get' command.",
    },
    {
        .name = "force", .key = 'f', .arg = NULL,
        .flags = 0, .group = 0,
        .doc = "Delete without prompting with 'del' command.",
    },
    {0} // Note: argp uses this to determine the array size
};

struct ParseResult* options_parse_result_init()
{
    struct ParseResult* const result = calloc(1, sizeof(*result));
    result->options = calloc(OPT_MAXIMUM, sizeof(*result->options));
    result->options->cmd = CMD_NO_CMD;
    return result;
}

void options_parse_result_clean(struct ParseResult* result)
{
    free(result->options);
    free(result);
}

static const struct argp ARGP = {
    .options = OPTIONS,
    .parser = parse_opt,
    .args_doc = ARG_DOC,
    .doc = DOC,
    .children = 0,
    .help_filter = 0,
    .argp_domain = 0
};

int options_parse(
    struct ParseResult* result, int argc, char* argv[])
{
    return argp_parse(&ARGP, argc, argv, 0, 0, result);
}

static error_t parse_opt(int key, char* const arg, struct argp_state* state)
{
    // input contains our Options struct
    struct ParseResult* const result = state->input;
    struct Options* const opts = result->options;

    switch (key)
    {
    case 'e':
        opts->holders[OPT_ECHO].is_active = true;
        break;
    case 'd':
    {
        struct OptionHolder* const holder = &opts->holders[OPT_DB_PATH];
        holder->is_active = true;
        holder->arg = arg;
        break;
    }
    case 'c':
    {
        struct OptionHolder* const holder = &opts->holders[OPT_CATEGORY];
        holder->is_active = true;
        holder->arg = arg;
        break;
    }
    case 'x':
        opts->holders[OPT_CLIP].is_active = true;
        break;
    case 'f':
        opts->holders[OPT_FORCE].is_active = true;
        break;
    case ARGP_KEY_ARG:
    {
        int parse_error = parse_arg(arg, state, result);
        if (parse_error)
        {
            return parse_error;
        }
        if (!parse_check_command_support(opts->cmd))
        {
            result->status = PARSE_UNSUPPORTED_CMD;
            return EOPNOTSUPP;
        }
        break;
    }
    case ARGP_KEY_END:
        if (opts->cmd == CMD_NO_CMD)
        {
            result->status = PARSE_NO_CMD;
            return EINVAL;
        }
        if (!parse_check_cmd_arg_count(opts->cmd, state->arg_num))
        {
            result->status = PARSE_MISSING_ARGUMENT;
            return EINVAL;
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static error_t parse_arg(
    char* const arg,
    const struct argp_state* const state,
    struct ParseResult* const result)
{
    struct Options* const opts = result->options;

    // Note: +1 cmd excluded with args, but is still parsed
    if (state->arg_num >= MAX_ARGS + 1)
    {
        // Too many arguments
        argp_usage(state);
    }
    else if (state->arg_num == 0)
    {
        if ((opts->cmd = cmd_match_string(arg)) == CMD_NO_CMD)
        {
            result->status = PARSE_NO_CMD;
            return EINVAL;
        }
        return 0;
    }
    opts->args[state->arg_num - 1] = arg; // Note: see above
    return 0;
}

static bool parse_check_cmd_arg_count(enum Command cmd, unsigned int end_arg_num)
{
    switch (cmd)
    {
    case CMD_NEW:
        return end_arg_num > CMD_NEW_ARG_REQUIRED;
    case CMD_LOGIN:
        return end_arg_num > CMD_LOGIN_ARG_REQUIRED;
    case CMD_ADD:
        return end_arg_num > CMD_ADD_ARG_REQUIRED;
    case CMD_GET:
        return end_arg_num > CMD_GET_ARG_REQUIRED;
    case CMD_LIST:
        return end_arg_num > CMD_LIST_ARG_REQUIRED;
    case CMD_DEL:
        return end_arg_num > CMD_DEL_ARG_REQUIRED;
    case CMD_EDIT:
        return end_arg_num > CMD_EDIT_ARG_REQUIRED;
    default:
        return false;
    }
}

bool parse_check_command_support(enum Command cmd)
{
    // TODO: Change once implemented
    switch (cmd)
    {
    case CMD_NEW:
    case CMD_GET:
    case CMD_ADD:
    case CMD_LIST:
        return true;
    case CMD_LOGIN:
    case CMD_DEL:
    case CMD_EDIT:
    default:
        return false;
    }
}
