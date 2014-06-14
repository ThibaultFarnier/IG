#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include "ei_event.h"
#include "ei_callbacks.h"
#include "ei_picksurface.h"
#include "ei_utils.h"

static ei_widget_t* 			root_widget 		= NULL;
static ei_surface_t 			root_window 		= NULL;


void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{
    //<> initializes the hardware (calls \ref hw_init)
    hw_init();

    //<> registers all classes of widget and all geometry managers
    ei_frame_register_class();
    ei_button_register_class();
    ei_toplevel_register_class();

    ei_register_placer_manager();

    //<> creates the root window (either in a system window, or the entire screen)
    root_window = hw_create_window(main_window_size, fullscreen);


    //<> creates the root widget to access the root window
    root_widget = ei_widget_create("frame", NULL);
    //root_widget->screen_location.size = *main_window_size;
    ei_point_t p = ei_point_zero();
    ei_place(root_widget, NULL, &p.x, &p.y,
             &main_window_size->width, &main_window_size->height,
             NULL, NULL, NULL, NULL);

    //<> creates the offsreen surface to manage picking
    ei_init_picksurface(root_window, main_window_size, EI_FALSE);
}


void free_tree(ei_widget_t *ptr)
{
    if (ptr != NULL) {
        if (ptr->next_sibling != NULL)
            free_tree(ptr->next_sibling);
        if (ptr->children_tail != NULL)
            free_tree(ptr->children_head);

        ptr->wclass->releasefunc(ptr);
    }
}


void ei_app_free()
{
    free_tree(ei_app_root_widget());
    //root_widget=NULL; // or ei_frame_release(root_widget);
    //hw_quit();
}


void draw_tree(ei_widget_t *ptr, ei_surface_t pick_surface)
{
    //printf("> draw_tree\n");
    //printf("draw: %s\n", ptr->wclass->name);
    if (ptr != NULL) {
        //*ptr->geom_params->runfunc(ptr);
        if (ptr->parent != NULL) {
            ptr->wclass->drawfunc(ptr, ei_app_root_surface(), pick_surface, ptr->parent->content_rect);
        } else {
            // ??????
            ptr->wclass->drawfunc(ptr, ei_app_root_surface(), pick_surface, ptr->content_rect);
        }
        if (ptr->next_sibling != NULL)
            draw_tree(ptr->next_sibling, pick_surface);
        if (ptr->children_tail != NULL)
            draw_tree(ptr->children_head, pick_surface);
    }
    //printf("> draw_tree out\n");
}

/*void browse_tree(ei_widget_t *ptr, bool condition, ei_event_t event, ei_callback_t callback)
  {
  callbacks *to_call = get_dc(event.type);

  if (ptr != NULL) {
  if (ptr->parent != NULL) 
  callback(ptr, &event, to_call->elmt.user_param);

  if (ptr->next_sibling != NULL)
  draw_tree(ptr->next_sibling, ei_app_root_picksurface());
  if (ptr->children_tail != NULL)
  draw_tree(ptr->children_head, ei_app_root_picksurface());
  }
  }*/

// Affiche les adresses des différents callbacks enregistrés
void print_callbacks() {
    callbacks *ptr;
    for (uint32_t i = ei_ev_none; i < ei_ev_last; i++) {
        ptr = get_dc(i); printf("%d: %p ", i, ptr);
        while (ptr != NULL) {
            ptr = ptr->next;
            printf("%p ", ptr);
        }
        printf("\n");
    }
}

void is_there_something_to_do(ei_widget_t *widget, ei_event_t event, callbacks *to_call, ei_bool_t *refresh) {
    ei_bool_t exit = EI_FALSE;

    // On parcourt la liste to_call correspondant à l'event en cherchant si un des callback concerne le widget
    while ((!exit) // Au début c'est les tags
            && (to_call != NULL)
            && (to_call->elmt.tag != NULL)) {
        // On regarde si le tag est all ou celui du type de widget
        if (strcmp(to_call->elmt.tag, "all") == 0 ||
                strcmp(to_call->elmt.tag, widget->wclass->name) == 0) {
            exit = (*(to_call->elmt.callback))(widget, &event, to_call->elmt.user_param);
            *refresh = EI_TRUE;
        }
        to_call = to_call->next;
    }
    while ((!exit) // Après c'est les callbacks associés à un widget particulier
            && (to_call != NULL)
            && (to_call->elmt.widget != NULL)) {
        // Est-ce que le widget est concerné?
        if (widget == to_call->elmt.widget) {
            exit = (*(to_call->elmt.callback))(widget, &event, to_call->elmt.user_param);
            *refresh = EI_TRUE;
        }
        to_call = to_call->next;
    }
}

ei_bool_t stop = EI_FALSE;
void ei_app_run()
{
    ei_widget_t *widget; //= ei_app_root_widget()->children_head;
    ei_event_t event;
    callbacks *to_call;

    printf("Début du run!\n");

    //bool exit = false;
    ei_bool_t refresh = EI_TRUE;
    while (! stop) {
        if (refresh)
            draw_tree(ei_app_root_widget(), ei_app_root_picksurface());
        refresh = EI_FALSE;

        hw_event_wait_next(&event);
        to_call = get_dc(event.type);

        switch (event.type) {
            case ei_ev_none:
                // No event, used on an un-initialized structure.
                // Not supposed to happen
                printf("NO EVENT?! WAT DA FUCK IS HAPPENIN BRO? WAT DID U DO?\n");
                break;

            case ei_ev_app:
                // An application event, created by hw_event_post_app.
                printf("application event\n");
                printf("WHAT SHOULD I DO?!\n");
                break;

            case ei_ev_keydown:
                // A keyboard key has been pressed.
                printf("keydown event\n");
                is_there_something_to_do(widget, event, to_call, &refresh);
                break;

            case ei_ev_keyup:
                // A keyboard key has been released.
                printf("keyup event\n");
                is_there_something_to_do(widget, event, to_call, &refresh);

                /**  EMERGENCY EXIT  **/
                if (event.param.key.key_sym == SDLK_q)
                    stop = true;
                /** END OF EMERGENCY **/
                break;

            case ei_ev_mouse_buttondown:
                // A mouse button has been pressed.
                printf("mousedown event\n");
                widget = ei_widget_pick(&(event.param.mouse.where)); 

                assert(widget != NULL);
                is_there_something_to_do(widget, event, to_call, &refresh);
                break;

            case ei_ev_mouse_buttonup:
                // A mouse button has been released.
                printf("mouseup event\n");
                widget = ei_widget_pick(&(event.param.mouse.where)); 

                assert(widget != NULL);
                is_there_something_to_do(widget, event, to_call, &refresh);
                break;

            case ei_ev_mouse_move:
                // The mouse has moved.
                printf("movemouse event\n");
                widget = ei_widget_pick(&(event.param.mouse.where)); 

                assert(widget != NULL);
                is_there_something_to_do(widget, event, to_call, &refresh);
                break;

            case ei_ev_last:
                // Last event type, its value is the number of event types.
                // Not supposed to happen.
                printf("WAIT, WHAT? LAST EVENT?!\n");
                break;

            default:
                // Not supposed to happen, all defined events should have been filtered
                printf("WHAT EVENT TYPE IS THAT?!\n");
        }
    }
    printf("Fin du run!\nByebye <3\n");
}

void ei_app_invalidate_rect(ei_rect_t* rect)
{

}

void ei_app_quit_request()
{
    stop = EI_TRUE;
}

ei_widget_t* ei_app_root_widget()
{
    return root_widget;
}

ei_surface_t ei_app_root_surface()
{
    return root_window;
}
