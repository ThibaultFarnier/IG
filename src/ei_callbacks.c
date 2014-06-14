#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ei_callbacks.h"


/* Insert fonction in a callbacks list */
/* Array referencing the declared callbacks
 * The array is indexed with the id of the ei_eventtype_t type
 * Initialized at NULL since nothing is recorded at the beginning
 */
static callbacks* declared_callbacks[ei_ev_last] = {NULL};


callbacks* get_dc(ei_eventtype_t event)
{
    return declared_callbacks[event];
}



/* Insert tag-related callbacks at the beginning of the list
 * and widget-related callbacks at the end of the list
 */
static void insert(callbacks **list, callbacktype elmt)
{
    // Definition of a new container for the new element
    callbacks *new_list = malloc(sizeof(callbacks));
    new_list->elmt = elmt;

    assert(list != NULL);
    // Test: tag-related callback?
    if (elmt.tag != NULL) {
        // Insert at the beginning
        new_list->next = *list;
        *list = new_list;        
        // Test: widget-related callback?
    } else if (elmt.widget != NULL) {
        // Insert at the end
        callbacks *ptr = *list;
        new_list->next = NULL;
        if (ptr != NULL) {
            while (ptr->next != NULL) {
                ptr = ptr->next;
            }
            ptr->next = new_list;
        } else 
            *list = new_list;
        //printf("%p\n", list);
    } else {
        // NOT SUPPOSED TO HAPPEN!
        printf("PROBLEM: TAG IS NULL AND WIDGET IS NULL!\n");
    }
}

void set_dc(ei_eventtype_t eventtype,
        ei_widget_t*	widget,
        ei_tag_t		tag,
        ei_callback_t	callback,
        void*			user_param)
{
    callbacktype new_callback
        = {eventtype, widget, tag, callback, user_param};
    insert(&(declared_callbacks[eventtype]), new_callback);
}


static void delete(callbacks **list, callbacks *element, ei_bool_t *b)
{
    *b = EI_TRUE;

    assert(list != NULL);
    callbacks *ptr = *list;
    assert(ptr != NULL);
    if (ptr == element) {
        *list = (*list)->next;
        free(ptr);
    } else {
        while (ptr->next != NULL) {
            if (ptr->next == element)
                free(ptr->next);
                ptr->next = ptr->next->next;
        }
        ptr = ptr->next;
    }
}


void del_dc(ei_eventtype_t eventtype,
        ei_widget_t*	widget,
        ei_tag_t		tag,
        ei_callback_t	callback,
        void*			user_param)
{
    //printf("del_dc\n");
    callbacks *list = get_dc(eventtype);
    callbacks *ptr = get_dc(eventtype);
    //printf("freh- sizeof(ptr)=%lu\n", sizeof(ptr));

    ei_bool_t trouve = EI_FALSE;
    assert(ptr != NULL);
    // Test: tag-related callback?
    if (tag != NULL) {
        while ((ptr != NULL) && (ptr->elmt.widget == NULL)) {
            if (strcmp(ptr->elmt.tag, tag) == 0) {
                if (ptr->elmt.callback == callback) {
                    delete(&list, ptr, &trouve);
                    declared_callbacks[eventtype] = list;
                    return;
                }
                ptr = ptr->next;
            }
        }
        // Test: widget-related callback?
    } else if (widget != NULL) {
        while ((ptr != NULL) && (ptr->elmt.widget == NULL)) {
            ptr = ptr->next;
        }
        while (ptr != NULL) {
            if (ptr->elmt.widget == widget) {
                if (ptr->elmt.callback == callback)
                    delete(&list, ptr, &trouve);
                declared_callbacks[eventtype] = list;
            }
            ptr = ptr->next;
        }
    } else {
        // NOT SUPPOSED TO HAPPEN!
        printf("PROBLEM: TAG IS NULL AND WIDGET IS NULL!\n");
    }
}
