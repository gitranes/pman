#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

struct ConfigView
{
    FILE* file;
    char* active_db_path;
    bool login_auto_cache_enabled;
    time_t login_cache_timeout;
};

struct ConfigView* config_view_init();
void config_view_clean(struct ConfigView* view);

int config_open(struct ConfigView* view, const char* config_path);
int config_read(struct ConfigView* view);

/**
 * Used for writing a new configuration file from the view's content.
 * @param view
 * @param config_path
 * @return
 */
int config_new(struct ConfigView* view, const char* config_path);

/**
 * Update existing configuration file with changes.
 * @param view
 * @return
 */
int config_update(struct ConfigView* view);
