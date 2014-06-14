#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "ei_application.h"
#include "ei_widgettypes.h"
#include "ei_widgetclass.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_utils.h"
#include "ei_linked_point.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "ei_params_list.h"


static ei_widgetclass_t* classlist = NULL;

static ei_color_t BLUE = {0x00, 0x00, 0xff, 0xff};
static ei_color_t RED = {0xD5, 0x4D, 0x4D, 0xff};

//static ei_rect_t def_rect = {{0,0},{600,600}};

void ei_widgetclass_register(ei_widgetclass_t* widgetclass)
{
    if (widgetclass != NULL) {
        widgetclass->next = classlist;
        classlist = widgetclass;
    }
}

ei_widgetclass_t* ei_widgetclass_from_name(ei_widgetclass_name_t name)
{
    ei_widgetclass_t *list = classlist;
    while(list!=NULL){
        if (strcmp(list->name, name) == 0)
            return list;
        list=list->next;
    }
    return NULL;
}

static ei_color_t bright_color(ei_color_t color)
{
    ei_color_t new_color;
    int facteur = 20;

    if (color.red > 255 - facteur - 1) {
        new_color.red = 255;
    } else {
        new_color.red = color.red + facteur;
    }

    if (color.blue > 255 - facteur - 1) {
        new_color.blue = 255;
    } else {
        new_color.blue = color.blue + facteur;
    }

    if (color.green > 255 - facteur - 1) {
        new_color.green = 255;
    } else {
        new_color.green = color.green + facteur;
    }

    new_color.alpha = color.alpha;

    return new_color;
}

static ei_color_t dark_color(ei_color_t color)
{
    ei_color_t new_color;
    int facteur = 20;

    if (color.red < facteur) {
        new_color.red = 0;
    } else {
        new_color.red = color.red - facteur;
    }

    if (color.blue < facteur) {
        new_color.blue = 0;
    } else {
        new_color.blue = color.blue - facteur;
    }

    if (color.green < facteur) {
        new_color.green = 0;
    } else {
        new_color.green = color.green - facteur;
    }

    new_color.alpha = color.alpha;

    return new_color;
}

/*static void show_list(ei_linked_point_t* list)
  {
  ei_linked_point_t* show_list = list;
  printf("New_list\n");
  while (show_list != NULL) {
  printf("x = %i, y = %i\n", show_list->point.x, show_list->point.y);
  show_list = show_list->next;
  }
  }*/

static void ei_draw_relief(ei_widget_t* widget,
        ei_surface_t surface,
        ei_surface_t pick_surface,
        ei_rect_t* clipper,
        ei_linked_point_t* list,
        ei_linked_point_t* uplist,
        ei_linked_point_t* downlist,
        ei_linked_point_t* mainlist)
{

    /*recast de widget*/
    ei_frame_widget_t* widgetf = (ei_frame_widget_t*)widget;




    printf("TIBOOOOOOOOO\n");
    printf("classname : %s\n", widget->wclass->name);
    printf("content_rect :\n");
    printf("x = %i, y = %i, w = %i, h = %i\n", widget->content_rect->top_left.x,
            widget->content_rect->top_left.y, widget->content_rect->size.width,
            widget->content_rect->size.height);
    printf("screenlocation :\n");
    printf("x = %i, y = %i, w = %i, h = %i\n",
            widget->screen_location.top_left.x,
            widget->screen_location.top_left.y,
            widget->screen_location.size.width,
            widget->screen_location.size.height);
    printf("clipper :\n");
    printf("x = %i, y = %i, w = %i, h = %i\n",
            clipper->top_left.x,clipper->top_left.y, clipper->size.width,
            clipper->size.height);

    /*show_list(list);
      show_list(uplist);
      show_list(downlist);
      show_list(mainlist);*/

    assert(widgetf != NULL);
    /*printf("widgetf=%p\n", widgetf);
      printf("widgetf->frame_opt.color=%d\n", widgetf->frame_opt.color);*/

    ei_color_t color = widgetf->frame_opt.color;
    ei_color_t pick_color = *(widgetf->widget.pick_color);

    //hw_surface_lock(pick_surface);
    ei_draw_polygon(pick_surface, list, pick_color, clipper);
    //hw_surface_unlock(pick_surface);
    //hw_surface_update_rects(pick_surface, NULL);

    hw_surface_lock(surface);
    switch (widgetf->frame_opt.relief)
    {
        case(ei_relief_raised):
            ei_draw_polygon(surface, uplist, bright_color(color), clipper);
            ei_draw_polygon(surface, downlist, dark_color(color), clipper);
            ei_draw_polygon(surface, mainlist, color, clipper);
            break;


        case(ei_relief_sunken):
            ei_draw_polygon(surface, uplist, dark_color(color), clipper);
            ei_draw_polygon(surface, downlist, bright_color(color), clipper);
            ei_draw_polygon(surface, mainlist, color, clipper);
            break;

        case(ei_relief_none):
            ei_draw_polygon(surface, list, color, clipper);
            break;

        default:
            printf("Erreur dans le type de relief.\n");
    }
    ei_point_t first_point = widget->screen_location.top_left;


    // Gestion de l'affichage du texte
    if(widgetf->frame_opt.text != NULL){

        // Get size text
        int text_width;
        int text_height;// = text_rect.size.height;

        hw_text_compute_size(widgetf->frame_opt.text,
                widgetf->frame_opt.text_font,
                &text_width, &text_height);
        /* ei_rect_t text_rect = hw_surface_get_rect(
           hw_text_create_surface(widgetf->frame_opt.text,
           widgetf->frame_opt.text_font,
           &widgetf->frame_opt.text_color)
           );*/


        // get widget size
        int widget_width = widgetf->widget.screen_location.size.width;
        int widget_height = widgetf->widget.screen_location.size.height;

        //printf("TW:%i ? WW:%i = %f",text_width,widget_width,(float)widget_width/(float)text_width);
        if (text_width>widget_width){
            //printf("TW:%i > WW:%i = %f",text_width,widget_width,(float)widget_width/(float)text_width);
            widgetf->frame_opt.text_font =
                hw_text_font_create(ei_default_font_filename,
                        ei_style_normal,
                        ei_font_default_size*((float)widget_width/(float)text_width)
                        );
            hw_text_compute_size(widgetf->frame_opt.text,
                    widgetf->frame_opt.text_font,
                    &text_width, &text_height);
            //hw_text_font_free(&widgetf->frame_opt.text_font);
            /*text_rect = hw_surface_get_rect(
              hw_text_create_surface(widgetf->frame_opt.text,
              widgetf->frame_opt.text_font,
              &widgetf->frame_opt.text_color)
              );*/
            /*text_width  = text_rect.size.width;
              text_height = text_rect.size.height;*/
        }

        // Calcul du first point en fonction de l'anchor
        ei_anchor_t* anchor = &widgetf->frame_opt.text_anchor;
        if (anchor != NULL) {
            int centerx = first_point.x+widget_width/2-(text_width/2);
            int centery = first_point.y+(widget_height/2)-(text_height/2);
            switch (*anchor) {
                case ei_anc_none:
                    // Ne doit pas arriver a priori
                    printf("ANC_NONE?! WAT DA FUCK IS DAT?\n");
                    break;
                case ei_anc_center:
                    //first_point.x = first_point.x - widget_width / 2;
                    //first_point.y = first_point.y - widget_height / 2;
                    first_point.x=centerx;
                    first_point.y=centery;
                    break;
                case ei_anc_north:
                    first_point.x=centerx;
                    break;
                case ei_anc_northeast:
                    first_point.x = first_point.x+    widget_width-text_width;
                    break;
                case ei_anc_east:
                    first_point.x = first_point.x+    widget_width-text_width;
                    first_point.y = centery;
                    break;
                case ei_anc_southeast:
                    first_point.x = first_point.x+  widget_width-text_width;
                    first_point.y = first_point.y+widget_height-text_height;
                    break;
                case ei_anc_south:
                    first_point.x = centerx;
                    first_point.y = first_point.y+widget_height-text_height;
                    break;
                case ei_anc_southwest:
                    first_point.y = first_point.y+widget_height-text_height;
                    break;
                case ei_anc_west:
                    first_point.y=centery;
                    break;
                case ei_anc_northwest:
                    // Nothing to be done, since its our default case
                    break;
                default:
                    printf("DA FUCK IS THAT ANCHOR?!\n");
            }
        }

        // On affiche le texte au bon endroit
        ei_draw_text(surface,
                &first_point,
                widgetf->frame_opt.text,
                widgetf->frame_opt.text_font,
                &widgetf->frame_opt.text_color,
                clipper);
    }

    if(widgetf->frame_opt.img != NULL){
        //printf("LAURENT:image:%p\n",widgetf->frame_opt.img);


        hw_surface_lock(*(widgetf->frame_opt.img));
        hw_surface_lock(ei_app_root_surface());

        //ei_draw_polygon(*(widgetf->frame_opt.img),uplist,);
        ei_rect_t rect_img;
        if (widgetf->frame_opt.img_rect==NULL)
            rect_img = hw_surface_get_rect(*(widgetf->frame_opt.img));
        else{
            rect_img = **(widgetf->frame_opt.img_rect);
        }


        printf("LAURENT:imgW:%i\n",rect_img.size.width);
        printf("LAURENT:imgH:%i\n",rect_img.size.height);
        ei_rect_t rect_widget = widget->screen_location;
        printf("LAURENT:widW%i\n",rect_widget.size.width);
        printf("LAURENT:widH%i\n",rect_widget.size.height);

        if (rect_img.size.width>rect_widget.size.width || rect_img.size.height>rect_widget.size.height){
            rect_img.size.width=rect_widget.size.width;
            rect_img.size.height=rect_widget.size.height;
        }

        ei_copy_surface(
                ei_app_root_surface(),
                &rect_widget,
                *(widgetf->frame_opt.img),
                &rect_img,
                0.5);

        //surface = widgetf->frame_opt.img;
        hw_surface_unlock(*(widgetf->frame_opt.img));
        hw_surface_unlock(ei_app_root_surface());
        //hw_surface_update_rects(*(widgetf->frame_opt.img), NULL);

        //hw_surface_lock(widgetf->frame_opt.img);
        //hw_surface_get_rect(widgetf->frame_opt.img);
    }

    hw_surface_unlock(surface);
    hw_surface_update_rects(surface, NULL);
}

/*****************
 ** FRAME CLASS **/
static void* ei_frame_alloc()
{
    return calloc(1, sizeof(ei_frame_widget_t));
}

static void ei_frame_release(struct ei_widget_t *widget)
{
    ei_frame_widget_t* w = (ei_frame_widget_t*)widget;

    //printf("release frame\n");
    free(w->widget.pick_color);
    //free(w->widget.content_rect);
    //if (w->frame_opt.color != NULL)
    //free(w->frame_opt.color);
    if (w->frame_opt.text != NULL)
        free(w->frame_opt.text);
    if (w->frame_opt.text_font != NULL)
        free(w->frame_opt.text_font);
    //if (w->frame_opt.text_color != NULL)
    //free(w->frame_opt.text_color);
    if (w->frame_opt.img != NULL)
        free(w->frame_opt.img);
    if (w->frame_opt.img_rect != NULL)
        free(w->frame_opt.img_rect);
}



static void ei_frame_draw(ei_widget_t* widget, ei_surface_t surface,
        ei_surface_t pick_surface, ei_rect_t* clipper)
{
    assert(widget != NULL);
    //printf("widget=%p\n", widget);

    /*recast de widget*/
    ei_frame_widget_t* widgetf = (ei_frame_widget_t*)widget;

    ei_size_t size = widget->screen_location.size;
    ei_point_t first_point = widget->screen_location.top_left;

    /*création de la liste de points à tracer*/
    ei_linked_point_t* list = create_linked_point_frame(first_point,size,0);
    ei_linked_point_t* uplist = create_up_linked_point_frame(first_point,size);
    ei_linked_point_t* downlist = create_down_linked_point_frame(first_point, size);
    int border_width = widgetf->frame_opt.border_width;
    ei_linked_point_t* mainlist = create_linked_point_frame(first_point, size,border_width);

    ei_draw_relief(widget, surface, pick_surface, clipper,//&def_rect,
            list, uplist, downlist, mainlist);

    vider_linked_point(&list);
    vider_linked_point(&uplist);
    vider_linked_point(&downlist);
    vider_linked_point(&mainlist);
}

static void ei_frame_setdefaults(struct ei_widget_t *widget)
{
    /*on recast le widget pour avoir accès à tous les paramètres*/
    ei_frame_widget_t* widgetf = (ei_frame_widget_t*)widget;

    widgetf->frame_opt.color = BLUE;
    widgetf->frame_opt.relief = ei_relief_none;
    widgetf->frame_opt.border_width = 0;
    widgetf->frame_opt.text = NULL;
    widgetf->frame_opt.text_font = NULL;
    widgetf->frame_opt.img = NULL;
    widgetf->frame_opt.img_rect = NULL;
}

static void ei_frame_geomnotify(struct ei_widget_t *widget, ei_rect_t rect)
{
    widget->screen_location = rect;
}

void ei_frame_register_class()
{
    ei_widgetclass_t *frame = malloc(sizeof(ei_widgetclass_t));
    strcpy(frame->name, "frame");
    frame->allocfunc = &ei_frame_alloc;
    frame->releasefunc = &ei_frame_release;
    frame->drawfunc = &ei_frame_draw;
    frame->setdefaultsfunc = &ei_frame_setdefaults;
    frame->geomnotifyfunc = &ei_frame_geomnotify;
    frame->next = NULL;
    ei_widgetclass_register(frame);
}


/******************
 ** BUTTON CLASS **/
static void* ei_button_alloc()
{
    return calloc(1, sizeof(ei_button_widget_t));
}

static void ei_button_draw(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    /*recast de widget*/
    ei_button_widget_t* widgetf = (ei_button_widget_t*)widget;
    int r = widgetf->corner_radius;

    /*printf("red = %u, green = %u, blue = %u, alpha = %u\n", widgetf->frame_opt.color.red, widgetf->frame_opt.color.green, widgetf->frame_opt.color.blue, widgetf->frame_opt.color.alpha);*/

    ei_size_t size = widget->screen_location.size;
    ei_point_t first_point = widget->screen_location.top_left;

    /*printf("x = %i, y = %i, height = %i, witdth = %i\n", first_point.x, first_point.y, size.height, size.width);*/

    ei_linked_point_t* list;
    ei_linked_point_t* uplist;
    ei_linked_point_t* downlist;
    ei_linked_point_t* mainlist;
    int border_width;

    /*création de la liste de points à tracer*/
    if (r != 0) {
        list = create_linked_point_button(first_point,size,0,r);
        uplist = create_up_linked_point_button(first_point,size,r);
        downlist = create_down_linked_point_button(first_point, size,r);
        border_width = widgetf->frame_opt.border_width;
        mainlist = create_linked_point_button(first_point, size,border_width,r);
    } else {
        list = create_linked_point_frame(first_point,size,0);
        uplist = create_up_linked_point_frame(first_point,size);
        downlist = create_down_linked_point_frame(first_point, size);
        border_width = widgetf->frame_opt.border_width;
        mainlist = create_linked_point_frame(first_point, size,border_width);
    }

    // Calls function to draw text ->
    ei_draw_relief(widget, surface, pick_surface, clipper, //&def_rect,
            list, uplist, downlist, mainlist);


    vider_linked_point(&list);
    vider_linked_point(&uplist);
    vider_linked_point(&downlist);
    vider_linked_point(&mainlist);
}

static void ei_button_geomnotify(struct ei_widget_t *widget, ei_rect_t rect)
{
    widget->screen_location = rect;
}

static void ei_button_release(struct ei_widget_t *widget)
{
}

static void ei_button_setdefaults(struct ei_widget_t *widget)
{
}

ei_bool_t button_unp(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    //printf("okay2\n");
    ei_button_widget_t* widgetb = (ei_button_widget_t*)widget;
    widgetb->frame_opt.relief = ei_relief_raised;
    ei_unbind(ei_ev_mouse_buttonup, NULL, "button", &button_unp, NULL);

    return EI_FALSE;
}

ei_bool_t button_p(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    //printf("okay1\n");
    ei_button_widget_t* widgetb = (ei_button_widget_t*)widget;
    widgetb->frame_opt.relief = ei_relief_sunken;
    ei_bind(ei_ev_mouse_buttonup, NULL, "button", &button_unp, NULL);

    return EI_FALSE;
}

void ei_button_register_class()
{
    ei_widgetclass_t *button = malloc(sizeof(ei_widgetclass_t));
    strcpy(button->name, "button");
    button->allocfunc = &ei_button_alloc;
    button->releasefunc = &ei_button_release;
    button->drawfunc = &ei_button_draw;
    button->setdefaultsfunc = &ei_button_setdefaults;
    button->geomnotifyfunc = &ei_button_geomnotify;
    button->next = NULL;
    ei_widgetclass_register(button);

    ei_bind(ei_ev_mouse_buttondown, NULL, "button", &button_p, NULL);
}


/********************
 ** TOPLEVEL CLASS **/
static void* ei_toplevel_alloc()
{
    return calloc(1, sizeof(ei_toplevel_widget_t));
}

static void ei_toplevel_draw(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    /*recast de widget*/
    //ei_toplevel_widget_t* widgetf = (ei_toplevel_widget_t*)widget;

    /*printf("TIBOOOOOOOOO\n");
      printf("classname : %s\n", widget->wclass->name);
      printf("content_rect :\n");
      printf("x = %i, y = %i, w = %i, h = %i\n", widget->content_rect->top_left.x,
      widget->content_rect->top_left.y, widget->content_rect->size.width,
      widget->content_rect->size.height);
      printf("screenlocation :\n");
      printf("x = %i, y = %i, w = %i, h = %i\n",
      widget->screen_location.top_left.x,
      widget->screen_location.top_left.y,
      widget->screen_location.size.width,
      widget->screen_location.size.height);
      printf("clipper :\n");
      printf("x = %i, y = %i, w = %i, h = %i\n",
      clipper->top_left.x,clipper->top_left.y, clipper->size.width,
      clipper->size.height);*/

    ei_size_t size = widget->requested_size;
    ei_point_t first_point = widget->screen_location.top_left;
    //ei_color_t color = widgetf->color;
    ei_color_t BLUE = {0xEB, 0x79, 0xEB, 0xFF};
    ei_color_t color = BLUE;

    ei_linked_point_t *list = create_linked_point_toplevel(first_point, size);

    hw_surface_lock(surface);
    ei_draw_polygon(surface, list, color, clipper);
    hw_surface_unlock(surface);
    hw_surface_update_rects(surface, NULL);

    ei_draw_polygon(pick_surface, list, *widget->pick_color, clipper); //&def_rect);

    vider_linked_point(&list);
}

static void ei_toplevel_geomnotify(struct ei_widget_t *widget, ei_rect_t rect)
{
    widget->screen_location = rect;
}

static void ei_toplevel_release(struct ei_widget_t *widget)
{
}

static void ei_toplevel_setdefaults(struct ei_widget_t *widget)
{
}

void refresh_coord(ei_widget_t *widget, ei_event_t *event, ei_point_t *mouse)
{
    /*int new_x = widget->screen_location.top_left.x + event->param.mouse.where.x - mouse->x;
    int new_y = widget->screen_location.top_left.y + event->param.mouse.where.y - mouse->y;

    ei_anchor_t anchor = ei_anc_northwest;
    
    ei_place(widget,
             &anchor,
             &new_x,
             &new_y,
             &widget->screen_location.size.width,
             &widget->screen_location.size.height,
             NULL, NULL, NULL, NULL);
    printf("francois: widget = %s, widget.size = {%i, %i}, x = %i, new_x = %i, y = %i, new_y = %i\n",
           widget->wclass->name,
           widget->screen_location.size.width,
           widget->screen_location.size.height,
           widget->screen_location.top_left.x,
           new_x,
           widget->screen_location.top_left.y,
           new_y);*/

    
    ei_params wparams = give_params(widget->pick_id);
    *wparams.x = *wparams.x + event->param.mouse.where.x - mouse->x;
    *wparams.y = *wparams.y + event->param.mouse.where.y - mouse->y;
    
    ei_place(widget, wparams.anchor, wparams.x, wparams.y, wparams.width,
             wparams.height, wparams.rel_x, wparams.rel_y,
             wparams.rel_width, wparams.rel_height);

}

void refresh_sons(ei_widget_t *widget, ei_point_t *mouse) {
    if (widget != NULL) {
        /*ei_anchor_t anchor = ei_anc_northwest;
        
         int x1 = widget->screen_location.top_left.x + mouse->x;
         int y1 = widget->screen_location.top_left.y + mouse->y;
         if (widget->parent != NULL) {
         x1 = x1 - widget->parent->screen_location.top_left.x;
         y1 = y1 - widget->parent->screen_location.top_left.y;
         }
         
         ei_place(widget,
         &anchor,
         &x1, &y1,
         &widget->screen_location.size.width,
         &widget->screen_location.size.height,
         NULL, NULL, NULL, NULL);*/
        
        
        ei_params wparams = give_params(widget->pick_id);

        ei_place(widget, wparams.anchor, wparams.x, wparams.y, wparams.width,
                 wparams.height, wparams.rel_x, wparams.rel_y, wparams.rel_width,
                 wparams.rel_height);

        refresh_sons(widget->next_sibling, mouse);
        refresh_sons(widget->children_head, mouse);
    }
}

ei_bool_t move_window(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    ei_point_t *mouse = (ei_point_t *)user_param;

    refresh_coord(widget, event, mouse);

    if (widget->next_sibling != NULL)
        refresh_sons(widget->next_sibling, mouse);
    else
        refresh_sons(widget->children_head, mouse);

    mouse->x = event->param.mouse.where.x;
    mouse->y = event->param.mouse.where.y;

    return EI_TRUE;
}

ei_bool_t end_move_window(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    ei_unbind(ei_ev_mouse_move, NULL, "all", &move_window, NULL);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", &end_move_window, NULL);

    return EI_TRUE;
}

ei_bool_t init_move_window(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    ei_bind(ei_ev_mouse_buttonup, NULL, "all", &end_move_window, NULL);
    ei_point_t *mouse = malloc(sizeof(ei_point_t));
    mouse->x = event->param.mouse.where.x;
    mouse->y = event->param.mouse.where.y;
    ei_bind(ei_ev_mouse_move, NULL, "all", &move_window, (void *)mouse);

    return EI_TRUE;
}

void ei_toplevel_register_class()
{
    ei_widgetclass_t *toplevel = malloc(sizeof(ei_widgetclass_t));
    strcpy(toplevel->name, "toplevel");
    toplevel->allocfunc = &ei_toplevel_alloc;
    toplevel->releasefunc = &ei_toplevel_release;
    toplevel->drawfunc = &ei_toplevel_draw;
    toplevel->setdefaultsfunc = &ei_toplevel_setdefaults;
    toplevel->geomnotifyfunc = &ei_toplevel_geomnotify;
    toplevel->next = NULL;
    ei_widgetclass_register(toplevel);

    ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", &init_move_window, NULL);
}
