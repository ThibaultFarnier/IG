#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_widgetclass.h"
#include "hw_interface.h"
#include "ei_widgettypes.h"
#include "ei_params_list.h"

// PAS VISIBLE! faire .h
//static ei_widgetclass_t* geometrymanagerlist = NULL;


static ei_geometrymanager_t*    geometrymanagerlist = NULL;

void ei_geometrymanager_register (ei_geometrymanager_t* geometrymanager)
{
    //assert(geometrymanager != NULL);
    if (geometrymanager != NULL) {
        geometrymanager->next = geometrymanagerlist;
        geometrymanagerlist = geometrymanager;
    }
}



ei_geometrymanager_t* ei_geometrymanager_from_name (ei_geometrymanager_name_t name)
{
    //printf("çava!");
    ei_geometrymanager_t *list = geometrymanagerlist;
    while (list != NULL) {
        if(strcmp(list->name, name) == 0)
            return list;

        list = list->next;
    }
    return NULL;
}



void ei_geometrymanager_unmap (ei_widget_t* widget)
{
    // forget about widget
}

void ei_place (ei_widget_t* widget,
        ei_anchor_t*	anchor,
        int*			x,
        int*			y,
        int*			width,
        int*			height,
        float*      	rel_x,
        float*      	rel_y,
        float*      	rel_width,
        float*      	rel_height)
{
    //widget->geom_params; // type: ei_geometry_param_t
    
    assert(widget != NULL);
    
    ei_manage_params(widget->pick_id,
                     anchor,
                     x,
                     y,
                     width,
                     height,
                     rel_x,
                     rel_y,
                     rel_width,
                     rel_height);
    

    if (x != NULL) {
        //printf("%i\n",*x);
        if (widget->parent != NULL) {
            widget->screen_location.top_left.x = *x + widget->parent->screen_location.top_left.x;
        } else {
            widget->screen_location.top_left.x = *x;
        }
    } else {
        if (widget->parent != NULL) {
            widget->screen_location.top_left.x = widget->parent->screen_location.top_left.x;
        } else {
            widget->screen_location.top_left.x = 0;
        }
    }
    
    if (rel_x != NULL) {
        widget->screen_location.top_left.x =
            widget->screen_location.top_left.x + (widget->parent->screen_location.size.width * (*rel_x));
    }
    
    if (y != NULL) {
        //printf("%i\n",*y);
        if (widget->parent != NULL) {
            widget->screen_location.top_left.y = *y + widget->parent->screen_location.top_left.y;
        } else {
            widget->screen_location.top_left.x = *y;
        }
    } else {
        if (widget->parent != NULL) {
            widget->screen_location.top_left.y = widget->parent->screen_location.top_left.y;
        } else {
            widget->screen_location.top_left.y = 0;
        }
    }
    if (rel_y != NULL) {
        widget->screen_location.top_left.y =
            widget->screen_location.top_left.y + (widget->parent->screen_location.size.height * (*rel_y));
    }

    if (rel_width != NULL) {
        widget->screen_location.size.width = widget->parent->screen_location.size.width * (*rel_width);
    } else if (width != NULL) {
        widget->screen_location.size.width = *width;
    } else {
        //printf("NO SPECIFIED WIDTH\n");
        widget->screen_location.size.width = widget->requested_size.width;
    }
    if (rel_height != NULL) {
        widget->screen_location.size.height = widget->parent->screen_location.size.height * (*rel_height);
    } else if (height != NULL) {
        widget->screen_location.size.height = *height;
    } else {
        //printf("NO SPECIFIED HEIGHT\n");
        widget->screen_location.size.height = widget->requested_size.height;
    }

    if (anchor != NULL) {
        switch (*anchor) {
            case ei_anc_none:
                // Ne doit pas arriver a priori
                printf("ANC_NONE?! WAT DA FUCK IS DAT?\n");
                break;
            case ei_anc_center:
                widget->screen_location.top_left.x = widget->screen_location.top_left.x - widget->screen_location.size.width / 2;
                widget->screen_location.top_left.y = widget->screen_location.top_left.y - widget->screen_location.size.height / 2;
                break;
            case ei_anc_north:
                widget->screen_location.top_left.x = widget->screen_location.top_left.x - widget->screen_location.size.width / 2;
                break;
            case ei_anc_northeast:
                widget->screen_location.top_left.x = widget->screen_location.top_left.x - widget->screen_location.size.width;
                break;
            case ei_anc_east:
                widget->screen_location.top_left.x = widget->screen_location.top_left.x - widget->screen_location.size.width;
                widget->screen_location.top_left.y = widget->screen_location.top_left.y - widget->screen_location.size.height / 2;
                break;
            case ei_anc_southeast:
                widget->screen_location.top_left.x = widget->screen_location.top_left.x - widget->screen_location.size.width;
                widget->screen_location.top_left.y = widget->screen_location.top_left.y - widget->screen_location.size.height;
                break;
            case ei_anc_south:
                widget->screen_location.top_left.x = widget->screen_location.top_left.x - widget->screen_location.size.width / 2;
                widget->screen_location.top_left.y = widget->screen_location.top_left.y - widget->screen_location.size.height;
                break;
            case ei_anc_southwest:
                widget->screen_location.top_left.y = widget->screen_location.top_left.y - widget->screen_location.size.height;
                break;
            case ei_anc_west:
                widget->screen_location.top_left.y = widget->screen_location.top_left.y - widget->screen_location.size.height / 2;
                break;
            case ei_anc_northwest:
                // Nothing to be done, since its our default case
                break;
            default:
                printf("DA FUCK IS THAT ANCHOR?!\n");
        }
    }
    
    if (strcmp(widget->wclass->name, "toplevel") == 0) {
        /*ei_toplevel_widget_t *w = (ei_toplevel_widget_t*)widget;
        if (w->closable){
            //ei_place(widget->children_head, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }*/
        widget->content_rect = widget->content_rect;
    } else if (strcmp(widget->wclass->name, "frame") == 0) {
        widget->content_rect = &widget->screen_location;
    } else if (strcmp(widget->wclass->name, "button") == 0) {
        widget->content_rect = &widget->screen_location;
    }
}

void ei_register_placer_manager()
{
    // Declare the geometry manager
    ei_geometrymanager_t* placer;
    placer = malloc(sizeof(ei_geometrymanager_t));
    // name, runfunc, releasefunc
    strcpy(placer->name,"placer");
    placer->runfunc     = (ei_geometrymanager_runfunc_t)ei_place;
    placer->releasefunc = (ei_geometrymanager_runfunc_t)ei_place;
    placer->next        = NULL;
    // Add it to the library
    ei_geometrymanager_register(placer);
}
