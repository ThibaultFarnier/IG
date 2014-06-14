#ifndef EI_LINKED_POINT_H
#define EI_LINKED_POINT_H

#include <stdlib.h>
#include <assert.h>

#include "ei_types.h"
#include "ei_draw.h"

void inserer_tete_linked_point(ei_linked_point_t** list, int x, int y);

void vider_linked_point(ei_linked_point_t** list);

/*création d'un liste de points formant un rectangle en prenant en 
 compte la bordure*/
ei_linked_point_t* create_linked_point_frame(ei_point_t first_point, ei_size_t size, int border_width);

/*création de la partie supérieure d'un rectangle pour donner une 
 impression de relief*/
ei_linked_point_t* create_up_linked_point_frame(ei_point_t first_point, ei_size_t size);

/*création de la partie inférieure d'un rectangle pour donner une
 impression de relief*/
ei_linked_point_t* create_down_linked_point_frame(ei_point_t first_point, ei_size_t size);

/*------- BUTTON -------*/
/*création d'un liste de points formant un rectangle en prenant en
 compte la bordure*/
ei_linked_point_t* create_linked_point_button(ei_point_t first_point, ei_size_t size, int border_width, int r);

/*création de la partie supérieure d'un rectangle arrondi pour donner une
 impression de relief*/
ei_linked_point_t* create_up_linked_point_button(ei_point_t first_point, ei_size_t size, int r);

/*création de la partie inférieure d'un rectangle arrondi pour donner une
 impression de relief*/
ei_linked_point_t* create_down_linked_point_button(ei_point_t first_point, ei_size_t size, int r);

/*-------- TOPLEVEL --------*/
ei_linked_point_t* create_linked_point_toplevel(ei_point_t first_point, ei_size_t size);



#endif