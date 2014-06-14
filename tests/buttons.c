#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	printf("Click !\n");
	return EI_TRUE;
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_sym == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}

	return EI_FALSE;
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	button1, *button2;
	ei_size_t	button1_size		= {150,100};
	ei_size_t	button2_size		= {50,100};
	int		button1_x		= 50;
	int		button1_y		= 50;
    int     button2_x       = 150;
    int     button2_y       = 150;
	ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
	char*		button1_title		= "Mon premier Bouton !";
	char*		button2_title		= "Mon deuxième Bouton !";
	ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
	int		button_corner_radius	= 20;
	ei_relief_t	button_relief		= ei_relief_raised;
	int		button_border_width	= 6;
	ei_callback_t	button_callback 	= button_press;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    printf("bonjour\n");


	/* Create, configure and place the button on screen. */
	button1 = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button1, &button1_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, &button1_title, NULL, &button_text_color, NULL,
				 NULL, NULL, NULL, &button_callback, NULL);
	ei_place(button1, NULL, &button1_x, &button1_y, NULL, NULL, NULL, NULL, NULL, NULL );
	button2 = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure	(button2, &button2_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, &button1_title, NULL, &button_text_color, NULL,
				 NULL, NULL, NULL, &button_callback, NULL);
	ei_place(button2, NULL, &button2_x, &button2_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Hook the keypress callback to the event. */
	ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
	ei_app_free();

	return (EXIT_SUCCESS);
}
