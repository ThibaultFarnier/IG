#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_widgettypes.h"
#include "ei_geometrymanager.h"
#include "ei_event.h"
#include "ei_types.h"
#include "ei_application.h"
#include "ei_picksurface.h"

uint32_t id_counter = 1;

ei_color_t BLACK = {0, 0, 0, 0xff};
//ei_color_t RED = {0xef, 0x4e, 0x4e, 0xff};

ei_color_t* choose_color(uint32_t id)
{
    ei_color_t* color = malloc(sizeof(ei_color_t));
    color->red = 0;
    color->green = 0;
    color->blue = 0;
    color->alpha = 0xff;

    if (id <= 255) {
        color->green = id;
    } else if (id <= 510) {
        color->red = (id - 255);
        color->green = 255;
    } else if (id <= 765) {
        color->red = 255;
        color->green = 255;
        color->blue = (id - 510);
    } else {
        // Ne doit pas arriver souvent...
        printf("COLOR OVERFLOW!\n");
    }
    //printf("R: %i, G: %i, B: %i\n", color->red, color->green, color->blue);
    return color;
}

void ei_insert_child(ei_widget_t *father, ei_widget_t *son)
{
    //printf("father: %p, son: %p\n", father, son);
    if (father != NULL) {
        if (father->children_head == NULL) {
            father->children_head = son;
        } else {
            father->children_tail->next_sibling = son;
        }
        father->children_tail = son;
        son->next_sibling = NULL;
    }
}

ei_widget_t* ei_widget_create(ei_widgetclass_name_t  class_name,
        ei_widget_t*           parent)
{
    ei_widgetclass_t* class = ei_widgetclass_from_name(class_name);
    assert(class != NULL);

    ei_widget_t* widget = class->allocfunc();

    //printf("id_counter: %i\n", id_counter);
    widget->wclass         = class;
    widget->pick_id        = id_counter++;
    widget->pick_color     = choose_color(widget->pick_id);
    if (parent != NULL) {
        if (strcmp(parent->wclass->name, "toplevel") != 0) {
            widget->parent = parent;
        } else {
            if (parent->children_head != NULL) {
                if (parent->children_head->next_sibling == parent->children_tail) {
                    widget->parent = parent->children_head;
                    parent = parent->children_head;
                    // on place le widget en fils du frame du toplevel
                }
            } else {
                widget->parent = parent;
            }
        }
    }

    widget->children_head  = NULL;
    widget->children_tail  = NULL;
    widget->next_sibling   = NULL;
    widget->geom_params    = ei_geometrymanager_from_name("placer");
    widget->requested_size.width        = 1;//600;
    widget->requested_size.height       = 1;//600;
    widget->screen_location.top_left.x  = 0;
    widget->screen_location.top_left.y  = 0;
    widget->screen_location.size.width  = 0;
    widget->screen_location.size.height = 0;
    widget->content_rect   = &(widget->screen_location);

    ei_insert_child(parent, widget);
    return widget;
}

void ei_widget_destroy(ei_widget_t* widget)
{

}

ei_widget_t* find_widget(ei_widget_t *ptr, ei_color_t id_color)
{
    assert(ptr != NULL);
    assert(ptr->pick_color != NULL);
    if ((ptr->pick_color->red == id_color.red)
            && (ptr->pick_color->green == id_color.green)
            && (ptr->pick_color->blue == id_color.red))
        return ptr;
    else {
        ei_widget_t *res1, *res2;
        if (ptr->next_sibling != NULL)
            res1 = find_widget(ptr->next_sibling, id_color);
        else
            res1 = NULL;
        if (ptr->children_tail != NULL)
            res2 = find_widget(ptr->children_head, id_color);
        else
            res2 = NULL;

        if (res1 == NULL)
            return res2;
        else
            return res1;
    }
    return NULL;
}

ei_widget_t* ei_widget_pick(ei_point_t* where)
{
    int ia = 0, ib = 0, ig = 0, ir = 0;
    ei_surface_t pick_surface = ei_app_root_picksurface();
    hw_surface_get_channel_indices(pick_surface, &ir, &ig, &ib, &ia);
    ei_size_t size = hw_surface_get_size(pick_surface);
    uint8_t *ptr = hw_surface_get_buffer(pick_surface);

    ptr = ptr + 4 * (where->y * size.width + where->x);
    //printf("ptr : %p\n", ptr);
    //printf("ir: %i\n", ir);
    //printf("ig: %i\n", ig);
    //printf("ib: %i\n", ib);
    //printf("rouge: %x, vert: %x, bleu: %x\n", *(ptr+ir), *(ptr+ig), *(ptr+ib));

    ei_color_t color = {*(ptr+ir), *(ptr+ig), *(ptr+ib), 0xFF};
    return find_widget(ei_app_root_widget(), color);
}

void ei_frame_configure(ei_widget_t* widget,
        ei_size_t*             requested_size,
        const ei_color_t*      color,
        int*                   border_width,
        ei_relief_t*           relief,
        char**                 text,
        ei_font_t*             text_font,
        ei_color_t*            text_color,
        ei_anchor_t*           text_anchor,
        ei_surface_t*          img,
        ei_rect_t**            img_rect,
        ei_anchor_t*           img_anchor)
{
    // Cast from widget to frame widget
    //printf("%ld\n", sizeof(ei_frame_widget_t));
    ei_frame_widget_t* w    = (ei_frame_widget_t*)widget;

    // A frame widget is also a widget, we need to initialize it
    //w->widget               = *(ei_widget_create("frame", widget->parent));

    if (requested_size != NULL) w->widget.requested_size =  *requested_size;
    //screen_location?
    //content_rect?

    // General options for the frame
    if (color != NULL)
        w->frame_opt.color        =  *color; // should be colour
    else
        w->frame_opt.color        = ei_default_background_color;
    if (relief != NULL)
        w->frame_opt.relief       = *relief;
    else
        w->frame_opt.relief       =  ei_relief_none;
    if (border_width != NULL)
        w->frame_opt.border_width = *border_width;
    else
        w->frame_opt.border_width =  0;
    if (text != NULL)
        w->frame_opt.text         = *text;
    else
        w->frame_opt.text         =  NULL;
    if (text_font != NULL)
        w->frame_opt.text_font    = *text_font;
    else
        w->frame_opt.text_font    =  ei_default_font;
    if (text_color != NULL)
        w->frame_opt.text_color   = *text_color;
    else
        w->frame_opt.text_color   =  ei_font_default_color;
    if (text_anchor != NULL)
        w->frame_opt.text_anchor  = *text_anchor;
    else
        w->frame_opt.text_anchor  =  ei_anc_northwest;
    w->frame_opt.img              =  img;
    if (img_rect != NULL)
        w->frame_opt.img_rect     = img_rect;
    if (img_anchor != NULL)
        w->frame_opt.img_anchor   = *img_anchor;
    else
        w->frame_opt.img_anchor   =  ei_anc_northwest; 


    // Geometry manager!
    // ei_frame_widget_t* -> ei_widget_t . ei_geometry_param_t -> ei_geometrymanager_t*
    //                                    =  ei_geometrymanager_t*
    ei_geometrymanager_t* geomgr = ei_geometrymanager_from_name("placer");

    w->widget.geom_params = malloc(sizeof(ei_geometry_param_t));
    w->widget.geom_params->manager = geomgr;

}


void ei_button_configure(ei_widget_t* widget,
        ei_size_t*             requested_size,
        const ei_color_t*      color,
        int*                   border_width,
        int*                   corner_radius,
        ei_relief_t*           relief,
        char**                 text,
        ei_font_t*             text_font,
        ei_color_t*            text_color,
        ei_anchor_t*           text_anchor,
        ei_surface_t*          img,
        ei_rect_t**            img_rect,
        ei_anchor_t*           img_anchor,
        ei_callback_t*         callback,
        void**                 user_param)
{
    // Cast from widget to button widget
    ei_button_widget_t* w   = (ei_button_widget_t*)widget;

    // A button widget is also a widget, we need to initialize it
    //w->widget               = *(ei_widget_create("button", widget->parent));

    // Widget parameters
    if (requested_size != NULL)
        w->widget.requested_size =  *requested_size;

    // Button parameters but common fields with frame
    if (color != NULL)
        w->frame_opt.color        =  *color;
    else
        w->frame_opt.color        = ei_default_background_color;
    if (relief != NULL)
        w->frame_opt.relief       = *relief;
    else
        w->frame_opt.relief       =  ei_relief_none;
    if (border_width != NULL)
        w->frame_opt.border_width = *border_width;
    else
        w->frame_opt.border_width =  0;
    if (text != NULL)
        w->frame_opt.text         = *text;
    else
        w->frame_opt.text         =  NULL;

    if (text_font != NULL) {
        w->frame_opt.text_font    =  text_font;
    } else {
       w->frame_opt.text_font = hw_text_font_create(ei_default_font_filename, ei_style_normal, ei_font_default_size);
    }

    if (text_color != NULL) {
        w->frame_opt.text_color   = *text_color;
    } else {
        w->frame_opt.text_color   =  BLACK;
    }
    if (text_anchor != NULL) {
        w->frame_opt.text_anchor  = *text_anchor;
    } else {
        w->frame_opt.text_anchor  =  ei_anc_center;
    }

    w->frame_opt.img              =  img;
    
    if (img_rect != NULL){
        w->frame_opt.img_rect     = img_rect;
        printf("ei_button_configure.c WIDTH:%i\n",(*img_rect)->size.width);
    }
    else w->frame_opt.img_rect = NULL;
    if (img_anchor != NULL)
        w->frame_opt.img_anchor   = *img_anchor;
    else
        w->frame_opt.img_anchor   =  ei_anc_northwest;

    // Button parameters

    if (corner_radius != NULL)
        w->corner_radius = *corner_radius;
    else
        w->corner_radius = 0;

    if (callback != NULL)
        w->callback = *callback;
    else
        w->callback = NULL;
    
    if (user_param != NULL)
        w->user_param = *user_param;
    else
        w->user_param = NULL;

    // callback binding
    if (w->callback != NULL) {
        ei_bind(ei_ev_mouse_buttondown, widget, NULL, w->callback, NULL);
    }
}

ei_color_t RED = {0xef, 0x4e, 0x4e, 0xff};
ei_color_t REDn = {0xef, 0x4e, 0x4e, 0x00};
int button_border_width = 3;
int r = 6;
ei_relief_t button_relief = ei_relief_raised;
ei_callback_t* b_callback = NULL;
int button_x = 5;
int button_y = - 30;
int button_width = 15;

void ei_toplevel_configure(ei_widget_t* widget,
        ei_size_t*             requested_size,
        ei_color_t*            color,
        int*                   border_width,
        char**                 title,
        ei_bool_t*             closable,
        ei_axis_set_t*         resizable,
        ei_size_t**            min_size)
{
    // Cast from widget to toplevel widget
    //printf("%ld\n", sizeof(ei_toplevel_widget_t));
    ei_toplevel_widget_t* w    = (ei_toplevel_widget_t*)widget;

    // A toplevel widget is also a widget, we need to initialize it
    //w->widget = *(ei_widget_create("toplevel", widget->parent));

    if (requested_size != NULL) w->widget.requested_size = *requested_size;

    if (color != NULL)
        w->color = *color;
    else
        w->color = ei_default_background_color;

    if (border_width != NULL)
        w->border_width = *border_width;
    else
        w->border_width = 0;

    if (*title != NULL)
        w->title = *title;
    else
        w->title = "Sans titre";

    if (closable != NULL) w->closable = *closable;
    
    ei_widget_t* toplevel_frame = ei_widget_create("frame", widget);
    ei_insert_child(widget, toplevel_frame);
    int frame_width = (*requested_size).width - 2 * (w->border_width);
    int frame_height = (*requested_size).height - (w->border_width) - 45;
    ei_frame_configure(toplevel_frame, NULL, color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    int frame_x = w->border_width;
    int frame_y = 45;
    ei_place(toplevel_frame, NULL, &frame_x, &frame_y, &frame_width, &frame_height, NULL, NULL, NULL, NULL);
    
    ei_widget_t* top_left_button = ei_widget_create("button", widget);
    ei_insert_child(widget, top_left_button);
    
    if (w->closable) {
        ei_button_configure(top_left_button, NULL, &RED, &button_border_width, &r, &button_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL, b_callback, NULL);
    } else {
        ei_button_configure(top_left_button, NULL, &REDn, &button_border_width, &r, &button_relief, NULL, NULL, NULL, NULL, NULL, NULL, NULL, b_callback, NULL);
    }
    ei_place(top_left_button, NULL, &button_x, &button_y, &button_width, &button_width, NULL, NULL, NULL, NULL);
    
    
    
    //ei_widget_t* toplevel_frame = ei_widget_create("frame", widget);

    if (resizable != NULL) w->resizable = *resizable;

    if (min_size != NULL) w->min_size = *min_size;
}
