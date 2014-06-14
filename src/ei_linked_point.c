#include <tgmath.h>
#include <stdio.h>

#include "ei_linked_point.h"

#define PI 3.14159

void inserer_tete_linked_point(ei_linked_point_t** list, int x, int y)
{
    ei_linked_point_t *suite = malloc(sizeof(ei_linked_point_t));
    assert(suite != NULL);
    ei_point_t p = {x,y};
    suite->point = p;
    suite->next = *list;
    *list=suite;
}

void vider_linked_point(ei_linked_point_t** list)
{
    while (*list != NULL) {
        ei_linked_point_t *suiv = (*list)->next;
        ei_linked_point_t *tmp = *list;
        *list = suiv;
        free(tmp);
    }
}

ei_linked_point_t* create_linked_point_frame(ei_point_t first_point,
                                             ei_size_t size,
                                             int border_width)
{
    ei_linked_point_t* list = NULL;
    inserer_tete_linked_point(&list,
                              first_point.x + border_width,
                              first_point.y + border_width);
    inserer_tete_linked_point(&list,
                              first_point.x + size.width-border_width,
                              first_point.y + border_width);
    inserer_tete_linked_point(&list,
                              first_point.x + size.width-border_width,
                              first_point.y + size.height-border_width);
    inserer_tete_linked_point(&list,
                              first_point.x + border_width,
                              first_point.y + size.height-border_width);
    return list;
}

ei_linked_point_t* create_up_linked_point_frame(ei_point_t first_point,
                                                ei_size_t size)
{
    ei_linked_point_t* list = NULL;
    inserer_tete_linked_point(&list, first_point.x, first_point.y);
    inserer_tete_linked_point(&list,
                              first_point.x + size.width,
                              first_point.y);
    if (size.width < size.height) {
        inserer_tete_linked_point(&list,
                                  first_point.x + size.width/2,
                                  first_point.y + size.width/2);
        inserer_tete_linked_point(&list,
                                  first_point.x + size.width/2,
                                  first_point.y + size.height - size.width/2);
    } else { 
        inserer_tete_linked_point(&list,
                                  first_point.x + size.width - size.height/2,
                                  first_point.y + size.height/2);
        inserer_tete_linked_point(&list,
                                  first_point.x + size.height/2,
                                  first_point.y + size.height/2);
    }
    inserer_tete_linked_point(&list,
                              first_point.x,
                              first_point.y + size.height);
    return list;
}

ei_linked_point_t* create_down_linked_point_frame(ei_point_t first_point, ei_size_t size)
{
    ei_linked_point_t* list = NULL;
    inserer_tete_linked_point(&list,
                              first_point.x + size.width,
                              first_point.y + size.height);
    inserer_tete_linked_point(&list,
                              first_point.x + size.width,
                              first_point.y);
    if (size.width < size.height) {
        inserer_tete_linked_point(&list,
                                  first_point.x + size.width/2,
                                  first_point.y + size.width/2);
        inserer_tete_linked_point(&list,
                                  first_point.x + size.width/2,
                                  first_point.y + size.height - size.width/2);
    } else {
        inserer_tete_linked_point(&list,
                                  first_point.x + size.width - size.height/2,
                                  first_point.y + size.height/2);
        inserer_tete_linked_point(&list,
                                  first_point.x + size.height/2,
                                  first_point.y + size.height/2);
    }
    inserer_tete_linked_point(&list,
                              first_point.x,
                              first_point.y + size.height);
    return list;
}


void arc(ei_linked_point_t** list, ei_point_t center, int radius, float begin_angle, float end_angle)
{
    int division = radius * 9/10;
    int i;
    /*printf("Le rayon est %d\n", radius);*/
    /*for (i=0; i<division; i++) {
        inserer_tete_linked_point(list,
                                  center.x + radius * cos(PI * i/(2*division)),
                                  center.y + radius * sin(PI * i/(2*division)));
        
    }*/
    float incr = (end_angle - begin_angle)/division;
    for (i=0; i < division; i++) {
        //printf("i*incr =  %f \n", (i*incr));
        inserer_tete_linked_point(list,
                                  center.x + radius * cos((float)(begin_angle + i*incr)),
                                  center.y - radius * sin((float)(begin_angle + i*incr)));
        //double x = center.x + radius * cos(begin_angle + i*incr);
        //double y = center.y - radius * sin(begin_angle + i*incr);
        //printf("x = %f ; y = %f\n", x, y);
    }
}

ei_linked_point_t* create_linked_point_button(ei_point_t first_point, ei_size_t size, int border_width, int r)
{
    ei_linked_point_t *list = NULL;
    /*int r = 0;*/
    int x = first_point.x;
    int y = first_point.y;
    int w = size.width;
    int h = size.height;
    ei_point_t c = {x + r + border_width, y + r + border_width};
    arc(&list, c, r , PI, PI/2);
    inserer_tete_linked_point(&list, x + w - border_width - r, y + border_width);
    c.x = x + w - r - border_width;
    c.y = y + r + border_width;
    arc(&list, c, r , PI/2, 0);
    inserer_tete_linked_point(&list, x + w - border_width, y + border_width + r);
    inserer_tete_linked_point(&list, x + w - border_width, y + h - border_width - r);
    c.x = x + w - r - border_width;
    c.y = y + h - r - border_width;
    arc(&list, c, r , 0, -PI/2);
    inserer_tete_linked_point(&list, x + r + border_width, y + h - border_width);
    c.x = x + r + border_width;
    c.y = y + h - r - border_width;
    arc(&list, c, r , 3*PI/2, PI);
    
    return list;
}

ei_linked_point_t* create_up_linked_point_button(ei_point_t first_point, ei_size_t size, int r)
{
    ei_linked_point_t *list = NULL;
    /*int r = 0;*/
    int x = first_point.x;
    int y = first_point.y;
    int w = size.width;
    int h = size.height;
    
    ei_point_t c = {x + r, y + r};
    arc(&list, c, r, PI, PI/2);
    c.x = x + w - r;
    c.y = y + r;
    arc(&list, c, r, PI/2, PI/4);
    if (w < h) {
        inserer_tete_linked_point(&list, x + w/2, y + w/2);
        inserer_tete_linked_point(&list, x + w/2, y + h - w/2);
    } else {
        inserer_tete_linked_point(&list, x + w - h/2, y + h/2);
        inserer_tete_linked_point(&list, x + h/2, y + h/2);
    }
    c.x = x + r;
    c.y = y + h - r;
    arc(&list, c, r, 5*PI/4, PI);
    
    return list;
}

ei_linked_point_t* create_down_linked_point_button(ei_point_t first_point, ei_size_t size, int r)
{
    ei_linked_point_t *list = NULL;
    /*int r = 0;*/
    int x = first_point.x;
    int y = first_point.y;
    int w = size.width;
    int h = size.height;
    
    ei_point_t c = {x + w - r, y + r};
    arc(&list, c, r, PI/4, 0);
    c.x = x + w - r;
    c.y = y + h - r;
    arc(&list, c, r, 0, -PI/2);
    c.x = x + r;
    c.y = y + h - r;
    arc(&list, c, r, -PI/2, -3*PI/4);
    if (w < h) {
        inserer_tete_linked_point(&list, x + w/2, y + h - w/2);
        inserer_tete_linked_point(&list, x + w/2, y + w/2);
    } else {
        inserer_tete_linked_point(&list, x + h/2, y + h/2);
        inserer_tete_linked_point(&list, x + w - h/2, y + h/2);
    }
    
    return list;
}

ei_linked_point_t* create_linked_point_toplevel(ei_point_t first_point, ei_size_t size)
{
    ei_linked_point_t *list = NULL;
    int r = 10;
    int x = first_point.x;
    int y = first_point.y;
    int w = size.width;
    int h = size.height;
    
    ei_point_t c = {x + r, y + r};
    arc(&list, c, r , PI, PI/2);
    inserer_tete_linked_point(&list, x + w - r, y);
    c.x = x + w - r;
    c.y = y + r;
    arc(&list, c, r , PI/2, 0);
    inserer_tete_linked_point(&list, x + w, y + h);
    inserer_tete_linked_point(&list, x, y + h);
    
    return list;
}