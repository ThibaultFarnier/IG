#ifndef EI_PICKSURFACE_H
#define EI_PICKSURFACE_H


#include "ei_widget.h"
#include "ei_types.h"


void ei_init_picksurface(const ei_surface_t root,
        const ei_size_t *size, ei_bool_t force_alpha);

ei_surface_t ei_app_root_picksurface();


#endif
