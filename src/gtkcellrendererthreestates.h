/*
 *  GThumb
 *
 *  Copyright (C) 2001 The Free Software Foundation, Inc.
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

/* gtkcellrenderertoggle.h
 * Copyright (C) 2000  Red Hat, Inc.,  Jonathan Blandford <jrb@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_CELL_RENDERER_THREE_STATES_H__
#define __GTK_CELL_RENDERER_THREE_STATES_H__

#include <gtk/gtk.h>

#define GTK_TYPE_CELL_RENDERER_THREE_STATES			(gtk_cell_renderer_three_states_get_type ())
#define GTK_CELL_RENDERER_THREE_STATES(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CELL_RENDERER_THREE_STATES, GtkCellRendererThreeStates))
#define GTK_CELL_RENDERER_THREE_STATES_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CELL_RENDERER_THREE_STATES, GtkCellRendererthree_statesClass))
#define GTK_IS_CELL_RENDERER_THREE_STATES(obj)		        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CELL_RENDERER_THREE_STATES))
#define GTK_IS_CELL_RENDERER_THREE_STATES_CLASS(klass)	        (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CELL_RENDERER_THREE_STATES))
#define GTK_CELL_RENDERER_THREE_STATES_GET_CLASS(obj)           (G_TYPE_CHECK_CLASS_CAST ((obj), GTK_TYPE_CELL_RENDERER_THREE_STATES, GtkCellRendererthree_statesClass))

typedef struct _GtkCellRendererThreeStates GtkCellRendererThreeStates;
typedef struct _GtkCellRendererThreeStatesClass GtkCellRendererThreeStatesClass;

struct _GtkCellRendererThreeStates
{
  GtkCellRenderer parent;

  /*< private >*/
  guint state : 2;
  guint activatable : 1;
  guint radio : 1;
  guint has_third_state : 1;
};

struct _GtkCellRendererThreeStatesClass
{
  GtkCellRendererClass parent_class;

  void (* toggled) (GtkCellRendererThreeStates *cell_renderer_three_states,
		    const gchar                *path);
};

GtkCellRenderer *gtk_cell_renderer_three_states_new             (void);
guint            gtk_cell_renderer_three_states_get_next_state  (GtkCellRendererThreeStates *three_states);

#endif /* __GTK_CELL_RENDERER_THREE_STATES_H__ */