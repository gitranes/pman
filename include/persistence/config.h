#pragma once

#include <stdbool.h>
#include <time.h>

struct ConfigView
{
    time_t login_cache_timeout;
    bool login_auto_cache_enabled;
};

struct ConfigView* config_view_init();
void config_view_clean(struct ConfigView* view);

int config_read(struct ConfigView* view);
