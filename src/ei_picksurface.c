#include <stddef.h>

#include "ei_application.h"

static ei_surface_t pick_surface = NULL;




void ei_init_picksurface(const ei_surface_t root,
        const ei_size_t *size, ei_bool_t force_alpha)
{
    pick_surface = hw_surface_create(root, size, force_alpha);
}

ei_surface_t ei_app_root_picksurface()
{
    return pick_surface;
}
