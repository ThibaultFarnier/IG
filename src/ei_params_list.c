#include <stdio.h>

#include "ei_types.h"
#include "ei_params_list.h"

ei_params_list *params_list = NULL;

void inserer_tete_params_list(ei_params_list** list,
                              uint32_t nb,
                              ei_anchor_t* anchor,
                              int* x,
                              int* y,
                              int* width,
                              int* height,
                              float* rel_x,
                              float* rel_y,
                              float* rel_width,
                              float* rel_height)
{
    ei_params_list *suite = malloc(sizeof(ei_params_list));
    assert(suite != NULL);
    ei_params p = {nb,anchor,x,y,width,height,rel_x,rel_y,rel_width,rel_height};
    suite->params = p;
    suite->next = *list;
    *list=suite;
}

void ei_manage_params(uint32_t nb,
                        ei_anchor_t *anchor,
                        int *x,
                        int *y,
                        int *width,
                        int *height,
                        float *rel_x,
                        float *rel_y,
                        float *rel_width,
                        float *rel_height)
{
    // two cases : list has already been registered or not
    // Check if list has already been registered
    ei_params_list* list = params_list;
    ei_bool_t found = EI_FALSE;
    if (params_list!= NULL) {
        while (list != NULL && found == EI_FALSE) {
            if (list->params.nb == nb) {
                found = EI_TRUE;
            } else {
                list = list->next;
            }
        }
    }
    
    // 1st case : list hasn't been registered.
    if (found == EI_FALSE) {
        inserer_tete_params_list(&params_list,
                                 nb,
                                 anchor,
                                 x,
                                 y,
                                 width,
                                 height,
                                 rel_x,
                                 rel_y,
                                 rel_width,
                                 rel_height);
    } else {
        // 2nd case : list has already been registered
        // --> we update it
        list->params.x = x;
        list->params.y = y;
        list->params.width = width;
        list->params.height = height;
        list->params.rel_x = rel_x;
        list->params.rel_y = rel_y;
        list->params.rel_width = rel_width;
        list->params.rel_height = rel_height;
    }
}

ei_params give_params(uint32_t nb)
{
    ei_bool_t found = EI_FALSE;
    ei_params_list* list = params_list;
    
    while (found != EI_TRUE && list != NULL) {
        if (list->params.nb == nb) {
            found = EI_TRUE;
        } else {
            list = list->next;
        }
    }
    
    if (list == NULL && found == EI_FALSE) {
        printf("Erreur, paramètres géométriques non retrouvés");
    }
    
    return list->params;
}

ei_params_list* erase_params(ei_params_list* list, uint32_t nb)
{
    if (list == NULL) {
        return NULL;
    }
    
    if (list->params.nb == nb) {
        ei_params_list *tmp = list->next;
        free(list);
        //tmp = erase_params(list, nb);
        return tmp;
    } else {
        params_list->next = erase_params(list->next, nb);
        return list;
    }
}

void erase_parameters(uint32_t nb)
{
    ei_params_list* list = params_list;
    list = erase_params(list, nb);
}

void ei_empty_params_list() {
    while (params_list != NULL) {
        ei_params_list* suiv = params_list->next;
        ei_params_list* tmp = params_list;
        params_list = suiv;
        free(tmp);
    }
}