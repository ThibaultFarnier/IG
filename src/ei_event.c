#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ei_event.h"
#include "ei_callbacks.h"



/***** .H FONCTIONS *****/
void ei_bind(ei_eventtype_t	eventtype,
        ei_widget_t*	widget,
        ei_tag_t		tag,
        ei_callback_t	callback,
        void*			user_param)
{
    //printf("binding done!\n");
    set_dc(eventtype, widget, tag, callback, user_param);
    //printf("%d: %p\n", eventtype, get_dc(eventtype));
}


void ei_unbind(ei_eventtype_t eventtype,
        ei_widget_t*	  widget,
        ei_tag_t		  tag,
        ei_callback_t  callback,
        void*		  user_param)
{
    del_dc(eventtype, widget, tag, callback, user_param);
    //printf("unbinding done!\n");
}
