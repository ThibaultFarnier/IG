#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ei_event.h"
#include "ei_widget.h"
#include "../src/ei_callbacks.h"

/****************************************/
/*** DEFINITION DES VARIABLES DE TEST ***/
#define _EVENT_ ei_ev_keydown
#define _WIDGET_ NULL
#define _TAG_ "all"
#define _ARG_ NULL
/****************************************/

static ei_bool_t mycallback(ei_widget_t *widget,
        struct ei_event_t *event, void *user_param)
{
}


uint32_t number_of_registered_elmts(ei_eventtype_t event)
{
    uint32_t counter = 0;
    callbacks *ptr = get_dc(event); 

    while (ptr != NULL) {
        counter++;
        ptr = ptr->next;
    }

    return counter;
}


int main(void)
{
    printf("Projet C - IG, Copyright Ensimag 2014.\n");
    printf("Test de ei_event.c:\n");

    /** PREMIER TEST **/
    printf("Test1: vérification de la déclaration du tableau des events");
    for (ei_eventtype_t ev = ei_ev_none; ev < ei_ev_last; ev++) {
        if (number_of_registered_elmts(ev) == 0) {
            printf(".");
        } else {
            printf("_");
        }
    }
    printf("             OK!\n");
    /** FIN PREMIER TEST **/

    ei_bind(_EVENT_, _WIDGET_, _TAG_, &mycallback, _ARG_);

    /** DEUXIEME TEST **/
    printf("Test2: vérification de l'ajout d'un élément dans le tableau des events");
    for (ei_eventtype_t ev = ei_ev_none; ev < ei_ev_last; ev++) {
        if (ev == _EVENT_) {
            if (number_of_registered_elmts(ev) == 1) {
                printf("."); 
            } else {
                printf("_");
            }
        } else if (number_of_registered_elmts(ev) == 0) {
            printf(".");
        } else {
            printf("_");
        }
    }
    printf("  OK!\n");
    /** FIN DEUXIEME TEST **/

    /** TROISIEME TEST **/
    printf("Test3: vérification des informations stockées");
    if (get_dc(_EVENT_)->elmt.eventtype == _EVENT_) {
        printf(".");
    } else {
        printf("_");
    }
    if (get_dc(_EVENT_)->elmt.widget == _WIDGET_) {
        printf(".");
    } else {
        printf("_");
    }
    if (strcmp(get_dc(_EVENT_)->elmt.tag, _TAG_) == 0) {
        printf(".");
    } else {
        printf("_");
    }
    if (get_dc(_EVENT_)->elmt.callback == &mycallback) {
        printf(".");
    } else {
        printf("_");
    }

    printf("                              OK!\n");
    /** FIN TROISIEME TEST **/

    
    printf("Finito!\n");

    return 0;
}



