#ifndef EI_CALLBACKS_H
#define EI_CALLBACKS_H

#include "ei_event.h"
#include "ei_widget.h"

/* Define the type of elements in the list */
typedef struct {
    ei_eventtype_t eventtype;
    ei_widget_t*   widget;
    ei_tag_t       tag;
    ei_callback_t  callback;
    void*          user_param;
} callbacktype;

/* Define a list structure */
/* NB: the list is ordered:
 *     - first, the callbacks associated to a tag
 *     - then, the callbacks associated to a particular widget
 */
struct callbacks {
    callbacktype      elmt;
    struct callbacks* next;
};
typedef struct callbacks callbacks;


callbacks* get_dc(ei_eventtype_t event);
void set_dc(ei_eventtype_t	eventtype,
        ei_widget_t*	widget,
        ei_tag_t		tag,
        ei_callback_t	callback,
        void*			user_param);

void del_dc(ei_eventtype_t	eventtype,
        ei_widget_t*	widget,
        ei_tag_t		tag,
        ei_callback_t	callback,
        void*			user_param);
#endif
