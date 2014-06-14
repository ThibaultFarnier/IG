/**
 * @file   ei_widgettypes.h
 *
 * @brief  API for widget specifications. Contains the 'subtypes' of widget: frame, button and toplevel.
 *
 * Created by François Ehrhardt, Thibault Farnier & Laurent Fite on 02.06.14.
 * Copyright 2014 Ensimag. All rights reserved.
 */

#ifndef EI_WIDGETTYPES_H
#define EI_WIDGETTYPES_H

#include "ei_widgetclass.h"
#include "ei_widget.h"

/**
 * \brief Structure for common fileds between frame and button.
 */
typedef struct {
    ei_color_t color; ///< Color of the considered widget
    
    /* énuméré spécifiant le relief de l'image */
    ei_relief_t relief; ///< Enumerate type that specifies the relief of the widget
    int border_width; ///< Size of border (if any)

    /* texte et attributs spécifiques */
    char* text; ///< Text in the widget
    ei_font_t *text_font; ///< Font of the text
    ei_color_t text_color; ///< Color of the text
    ei_anchor_t text_anchor;

    /* image à dessiner à la place du texte */
    ei_surface_t *img; ///< ???
    ei_rect_t **img_rect; ///< ???
    ei_anchor_t img_anchor;
}common_fields;


/**
 * \brief  Fields proper to frame type widgets
 */
typedef struct {
    ei_widget_t widget; ///< The common parameters of all types of widgets.

    common_fields frame_opt; ///< Common paramaters between frame and button.
} ei_frame_widget_t;

/**
 * \brief  Fields proper to button type widgets
 */
typedef struct {
    /* attribus communs aux widgets */
    ei_widget_t widget; ///< The common parameters of all types of widgets.

    common_fields frame_opt; ///< Common paramaters between frame and button.
    /* adresse d'une fonction de traitant */
    
    int corner_radius; ///< Speaks for itself.
    
    ei_callback_t callback; ///< Address of the callee fonction. This fonction is called when the button is pressed.

    /* adresse mémoire permettant au programmeur de l'application de passer un param au traitant du bouton */
    void* user_param; ///< Memory address allowing the dev to give a paramater to the callee fonction.
} ei_button_widget_t;

/**
 * \brief Fields proper to toplevel widgets
 */
typedef struct {
    ei_widget_t widget; ///< The common parameters of all types of widgets.
    ei_color_t color; ///< Requested background color.
    int border_width; ///< Speaks for itself.
    char *title; ///< Title of toplevel widget.
    ei_bool_t closable; ///< Is it closable?
    ei_axis_set_t resizable; ///< Is it resizable?
    ei_size_t *min_size; ///< Speaks fot itself.
} ei_toplevel_widget_t;

#endif
