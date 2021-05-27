#include "persistence/config.h"

#include <stdlib.h>

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
