#include "options/parse.h"

#include <argp.h>

#include <stdlib.h>

static error_t parse_opt(int key, char* arg, struct argp_state* state);

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

static inline void options_parse_free_argv(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        free(argv[i]);
    }
    free(argv);
}

struct ParseResult* options_parse_result_init()
{
    struct ParseResult* const result = calloc(1, sizeof(*result));
    result->options = calloc(1, sizeof(*result->options));
    return result;
}

void options_parse_result_clean(struct ParseResult* result)
{
    struct Options* const opts = result->options;
    options_parse_free_argv(opts->argc, opts->argv);

    for (size_t i = 0; i < opts->holders.size; ++i)
    {
        struct OptionHolder* const holder = &opts->holders.buf[i];
        options_parse_free_argv(holder->argc, holder->argv);
    }

    free(opts);
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
    return argp_parse(&ARGP, argc, argv, 0, 0, result->options);
}

static error_t parse_opt(int key, char* const arg, struct argp_state* state)
{
    // input contains our Options struct
    struct Options* const opts = state->input;

    (void)(opts);
    char* arg_2 = arg;
    (void)(arg_2);

    switch (key)
    {
    case 'e':
        puts("echo");
        break;
    case 'd':
        puts("db-path");
        break;
    case 'c':
        puts("category");
        break;
    case 'x':
        puts("clip");
        break;
    case 'f':
        puts("force");
        break;

    case ARGP_KEY_ARG:
        if (state->arg_num >= 2)
        {
            // Too many arguments.
            argp_usage(state);
        }
        break;
    case ARGP_KEY_END:
        if (state->arg_num == 0)
        {
            // Not enough arguments.
            argp_usage(state);
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
