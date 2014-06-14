#ifndef EI_PARAMS_LIST_H
#define EI_PARAMS_LIST_H

#include <stdlib.h>
#include <assert.h>

typedef struct {
    uint32_t nb;
    ei_anchor_t* anchor;
    int* x;
    int* y;
    int* width;
    int* height;
    float* rel_x;
    float* rel_y;
    float* rel_width;
    float* rel_height;
} ei_params;

typedef struct ei_params_list {
    ei_params params;
    struct ei_params_list *next;
} ei_params_list;

void ei_manage_params(uint32_t nb,
                      ei_anchor_t* anchor,
                      int* x,
                      int* y,
                      int* width,
                      int* height,
                      float* rel_x,
                      float* rel_y,
                      float* rel_width,
                      float* rel_height);

ei_params give_params(uint32_t nb);

void ei_empty_params_list();

#endif