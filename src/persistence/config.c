#include "persistence/config.h"

#include <ini.h>

#include <stdio.h>
#include <stdlib.h>

static int config_ini_handler(
        void* user,
        const char* section,
        const char* name,
        const char* value);

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
    free(view);
}

int config_open(struct ConfigView* view, const char* config_path)
{
    FILE* ini_fp = NULL;
    if (!(ini_fp = fopen(config_path, "r")))
    {
        return -1;
    }
    if
}
