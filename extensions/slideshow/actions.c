/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/*
 *  GThumb
 *
 *  Copyright (C) 2009 Free Software Foundation, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Street #330, Boston, MA 02111-1307, USA.
 */


#include <config.h>
#include <glib/gi18n.h>
#include <gthumb.h>
#include "gth-slideshow.h"
#include "gth-transition.h"
#include "preferences.h"


void
gth_browser_activate_action_view_slideshow (GtkAction  *action,
					    GthBrowser *browser)
{
	GList     *items;
	GList     *file_list;
	GtkWidget *slideshow;
	char      *transition_id;
	GList     *transitions = NULL;

	items = gth_file_selection_get_selected (GTH_FILE_SELECTION (gth_browser_get_file_list_view (browser)));
	if ((items == NULL) || (items->next == NULL))
		file_list = gth_file_store_get_visibles (GTH_FILE_STORE (gth_browser_get_file_store (browser)));
	else
		file_list = gth_file_list_get_files (GTH_FILE_LIST (gth_browser_get_file_list (browser)), items);

	slideshow = gth_slideshow_new (browser, file_list);
	gth_slideshow_set_delay (GTH_SLIDESHOW (slideshow), eel_gconf_get_float (PREF_SLIDESHOW_CHANGE_DELAY, 5) * 1000);
	gth_slideshow_set_automatic (GTH_SLIDESHOW (slideshow), eel_gconf_get_boolean (PREF_SLIDESHOW_AUTOMATIC, TRUE));
	gth_slideshow_set_loop (GTH_SLIDESHOW (slideshow), eel_gconf_get_boolean (PREF_SLIDESHOW_WRAP_AROUND, FALSE));

	transition_id = eel_gconf_get_string (PREF_SLIDESHOW_TRANSITION, DEFAULT_TRANSITION);
	if (strcmp (transition_id, "random") == 0) {
		transitions = gth_main_get_registered_objects (GTH_TYPE_TRANSITION);
	}
	else {
		GthTransition *transition = gth_main_get_registered_object (GTH_TYPE_TRANSITION, transition_id);

		if (transition != NULL)
			transitions = g_list_append (NULL, transition);
		else
			transitions = NULL;
	}
	gth_slideshow_set_transitions (GTH_SLIDESHOW (slideshow), transitions);

	gtk_window_fullscreen (GTK_WINDOW (slideshow));
	/*gtk_window_set_default_size (GTK_WINDOW (slideshow), 700, 700);*/
	gtk_window_present (GTK_WINDOW (slideshow));

	_g_object_list_unref (transitions);
	g_free (transition_id);
	_g_object_list_unref (file_list);
	_gtk_tree_path_list_free (items);
}