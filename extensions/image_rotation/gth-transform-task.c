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
#include "gth-transform-task.h"
#include "rotation-utils.h"


struct _GthTransformTaskPrivate {
	GthBrowser    *browser;
	GCancellable  *cancellable;
	GList         *file_list;
	GList         *current;
	GthTransform   transform;
	JpegMcuAction  default_action;
};


static gpointer parent_class = NULL;


static void
gth_transform_task_finalize (GObject *object)
{
	GthTransformTask *self;

	self = GTH_TRANSFORM_TASK (object);

	_g_object_list_unref (self->priv->file_list);
	_g_object_unref (self->priv->cancellable);

	G_OBJECT_CLASS (parent_class)->finalize (object);
}


static void transform_current_file (GthTransformTask *self);


static void
transform_next_file (GthTransformTask *self)
{
	/*self->priv->default_action = JPEG_MCU_ACTION_ABORT;*/
	self->priv->current = self->priv->current->next;
	transform_current_file (self);
}


static void
trim_response_cb (JpegMcuAction action,
		  gpointer      user_data)
{
	GthTransformTask *self = user_data;

	if (action != JPEG_MCU_ACTION_ABORT) {
		self->priv->default_action = action;
		transform_current_file (self);
	}
	else
		transform_next_file (self);
}


static void
transform_file_ready_cb (GError   *error,
			 gpointer  user_data)
{
	GthTransformTask *self = user_data;

	if (error != NULL) {
		if (g_error_matches (error, JPEG_ERROR, JPEG_ERROR_MCU)) {
			GthFileData *file_data;

			g_clear_error (&error);

			file_data = self->priv->current->data;
			ask_whether_to_trim (GTK_WINDOW (self->priv->browser),
					     file_data,
					     trim_response_cb,
					     self);

			return;
		}

		gth_task_completed (GTH_TASK (self), error);
		return;
	}

	transform_next_file (self);
}


static void
transform_current_file (GthTransformTask *self)
{
	GthFileData *file_data;

	if (self->priv->current == NULL) {
		gth_task_completed (GTH_TASK (self), NULL);
		return;
	}

	file_data = self->priv->current->data;
	apply_transformation_async (file_data,
				    self->priv->transform,
				    self->priv->default_action,
				    self->priv->cancellable,
				    transform_file_ready_cb,
				    self);
}


static void
gth_transform_task_exec (GthTask *task)
{
	GthTransformTask *self;

	g_return_if_fail (GTH_IS_TRANSFORM_TASK (task));

	self = GTH_TRANSFORM_TASK (task);

	self->priv->current = self->priv->file_list;
	transform_current_file (self);
}


static void
gth_transform_task_cancel (GthTask *task)
{
	GthTransformTask *self;

	g_return_if_fail (GTH_IS_TRANSFORM_TASK (task));

	self = GTH_TRANSFORM_TASK (task);

	g_cancellable_cancel (self->priv->cancellable);
}


static void
gth_transform_task_class_init (GthTransformTaskClass *klass)
{
	GObjectClass *object_class;
	GthTaskClass *task_class;

	parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GthTransformTaskPrivate));

	object_class = G_OBJECT_CLASS (klass);
	object_class->finalize = gth_transform_task_finalize;

	task_class = GTH_TASK_CLASS (klass);
	task_class->exec = gth_transform_task_exec;
	task_class->cancel = gth_transform_task_cancel;
}


static void
gth_transform_task_init (GthTransformTask *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, GTH_TYPE_TRANSFORM_TASK, GthTransformTaskPrivate);
	self->priv->cancellable = g_cancellable_new ();
	self->priv->default_action = JPEG_MCU_ACTION_ABORT; /* FIXME: save a gconf value for this */
}


GType
gth_transform_task_get_type (void)
{
	static GType type = 0;

	if (! type) {
		GTypeInfo type_info = {
			sizeof (GthTransformTaskClass),
			NULL,
			NULL,
			(GClassInitFunc) gth_transform_task_class_init,
			NULL,
			NULL,
			sizeof (GthTransformTask),
			0,
			(GInstanceInitFunc) gth_transform_task_init
		};

		type = g_type_register_static (GTH_TYPE_TASK,
					       "GthTransformTask",
					       &type_info,
					       0);
	}

	return type;
}


GthTask *
gth_transform_task_new (GthBrowser   *browser,
			GList        *file_list,
			GthTransform  transform)
{
	GthTransformTask *self;

	self = GTH_TRANSFORM_TASK (g_object_new (GTH_TYPE_TRANSFORM_TASK, NULL));
	self->priv->browser = browser;
	self->priv->file_list = _g_object_list_ref (file_list);
	self->priv->transform = transform;

	return (GthTask *) self;
}