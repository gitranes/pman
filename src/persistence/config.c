#include "persistence/config.h"

#include <ini.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int TIMEOUT_BASE = 10;

static const char* const CONFIG_SECTION = "settings";
static const char* const CONFIG_ACTIVE_DB = "active_db_path";
static const char* const CONFIG_LOGIN_CACHE = "login_auto_cache";
static const char* const CONFIG_CACHE_TIMEOUT = "login_cache_timeout";

static int config_open_w_mode(
    struct ConfigView* view, const char* config_path, const char* mode);

static int config_ini_handler(
        void* user,
        const char* section,
        const char* name,
        const char* value);

static void config_write_view(const struct ConfigView* view);

static void config_write_db_path(const struct ConfigView* view);
static void config_write_login_cache_enabled(const struct ConfigView* view);
static void config_write_login_cache_timeout(const struct ConfigView* view);

struct ConfigView* config_view_init()
{
    return calloc(1, sizeof(struct ConfigView));
}

void config_view_clean(struct ConfigView* view)
{
    if (view->file)
    {
        fclose(view->file);
    }
    free(view->active_db_path);
    free(view);
}

int config_open(struct ConfigView* view, const char* config_path)
{
    if (config_open_w_mode(view, config_path, "r"))
    {
        return -1;
    }
    return 0;
}

int config_read(struct ConfigView* view)
{
    if (ini_parse_file(view->file, config_ini_handler, view) < 0)
    {
        return -1;
    }
    return 0;
}

int config_new(struct ConfigView* view, const char* config_path)
{
    if (config_open_w_mode(view, config_path, "w"))
    {
        return -1;
    }
    config_write_view(view);
    return 0;
}

int config_update(struct ConfigView* view)
{
    if (freopen(NULL, "w", view->file))
    {
        return -1;
    }
    config_write_view(view);
    return 0;
}

static int config_open_w_mode(
    struct ConfigView* view, const char* config_path, const char* mode)
{
    FILE* ini_fp = NULL;
    if (!(ini_fp = fopen(config_path, mode)))
    {
        return -1;
    }
    view->file = ini_fp;
    return 0;
}

static int config_ini_handler(
    void* user,
    const char* section,
    const char* name,
    const char* value)
{
    struct ConfigView* const view = user;
    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    if (MATCH(CONFIG_SECTION, CONFIG_ACTIVE_DB))
    {
        view->active_db_path = strdup(value);
    }
    else if (MATCH(CONFIG_SECTION, CONFIG_LOGIN_CACHE))
    {
        if (strcmp(value, "true") == 0)
        {
            view->login_auto_cache_enabled = true;
        }
        else
        {
            view->login_auto_cache_enabled = false;
        }
    }
    else if (MATCH(CONFIG_SECTION, CONFIG_CACHE_TIMEOUT))
    {
        view->login_cache_timeout = strtol(value, NULL, TIMEOUT_BASE);
    }
    return 0;
}

static void config_write_view(const struct ConfigView* view)
{
    fprintf(view->file, "[%s]\n", CONFIG_SECTION);

    config_write_db_path(view);
    config_write_login_cache_enabled(view);
    config_write_login_cache_timeout(view);
}

static void config_write_db_path(const struct ConfigView* view)
{
    if (view->active_db_path)
    {
        fprintf(
            view->file, "%s = %s\n", CONFIG_ACTIVE_DB, view->active_db_path);
    }
    else
    {
        fprintf(view->file, "%s =\n", CONFIG_ACTIVE_DB);
    }
}

static void config_write_login_cache_enabled(const struct ConfigView* view)
{
    if (view->login_auto_cache_enabled)
    {
        fprintf(view->file, "%s = %s\n", CONFIG_LOGIN_CACHE, "true");
    }
    else
    {
        fprintf(view->file, "%s = %s\n", CONFIG_LOGIN_CACHE, "false");
    }
}

static void config_write_login_cache_timeout(const struct ConfigView* view)
{
    fprintf(view->file,
        "%s = %ld\n", CONFIG_CACHE_TIMEOUT, view->login_cache_timeout);
}
