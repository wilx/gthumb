/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/*
 *  GThumb
 *
 *  Copyright (C) 2001-2008 Free Software Foundation, Inc.
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

#ifndef _GLIB_UTILS_H
#define _GLIB_UTILS_H

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>
#include "typedefs.h"

G_BEGIN_DECLS

#define GFILE_NAME_TYPE_ATTRIBUTES "standard::name,standard::type"
#define GFILE_DISPLAY_ATTRIBUTES "standard::display-name,standard::icon"
#define GFILE_BASIC_ATTRIBUTES GFILE_DISPLAY_ATTRIBUTES ",standard::name,standard::type"

#define DEFINE_STANDARD_ATTRIBUTES(a) ( \
	"standard::type," \
	"standard::is-hidden," \
	"standard::is-backup," \
	"standard::name," \
	"standard::display-name," \
	"standard::edit-name," \
	"standard::icon," \
	"standard::size," \
	"thumbnail::path" \
	"time::created," \
	"time::created-usec," \
	"time::modified," \
	"time::modified-usec," \
	"access::*" \
	a)
#define GFILE_STANDARD_ATTRIBUTES (DEFINE_STANDARD_ATTRIBUTES(""))
#define GFILE_STANDARD_ATTRIBUTES_WITH_FAST_CONTENT_TYPE (DEFINE_STANDARD_ATTRIBUTES(",standard::fast-content-type"))
#define GFILE_STANDARD_ATTRIBUTES_WITH_CONTENT_TYPE (DEFINE_STANDARD_ATTRIBUTES(",standard::fast-content-type,standard::content-type"))

#define GNOME_COPIED_FILES (gdk_atom_intern_static_string ("x-special/gnome-copied-files"))
#define IROUND(x) ((int)floor(((double)x) + 0.5))
#define FLOAT_EQUAL(a,b) (fabs (a - b) < 1e-6)
#define ID_LENGTH 8
#define G_TYPE_OBJECT_LIST (g_object_list_get_type ())
#define G_TYPE_STRING_LIST (g_string_list_get_type ())

/* signals */

#define g_signal_handlers_disconnect_by_data(instance, data) \
    g_signal_handlers_disconnect_matched ((instance), G_SIGNAL_MATCH_DATA, \
					  0, 0, NULL, NULL, (data))
#define g_signal_handlers_block_by_data(instance, data) \
    g_signal_handlers_block_matched ((instance), G_SIGNAL_MATCH_DATA, \
				     0, 0, NULL, NULL, (data))
#define g_signal_handlers_unblock_by_data(instance, data) \
    g_signal_handlers_unblock_matched ((instance), G_SIGNAL_MATCH_DATA, \
				       0, 0, NULL, NULL, (data))

/* gobject utils*/

gpointer      _g_object_ref                  (gpointer     object);
void          _g_object_unref                (gpointer     object);
void          _g_object_clear                (gpointer     object);
GList *       _g_object_list_ref             (GList       *list);
void          _g_object_list_unref           (GList       *list);
GType         g_object_list_get_type         (void);
GEnumValue *  _g_enum_type_get_value         (GType        enum_type,
					      int          value);
GEnumValue *  _g_enum_type_get_value_by_nick (GType        enum_type,
					      const char  *nick);

/* idle callback */

typedef struct {
	DoneFunc func;
	gpointer data;
} IdleCall;


IdleCall* idle_call_new           (DoneFunc       func,
				   gpointer       data);
void      idle_call_free          (IdleCall      *call);
guint     idle_call_exec          (IdleCall      *call,
				   gboolean       use_idle_cb);
guint     call_when_idle          (DoneFunc       func,
				   gpointer       data);
void      object_ready_with_error (gpointer       object,
				   ReadyCallback  ready_func,
				   gpointer       user_data,
				   GError        *error);
void      ready_with_error        (ReadyFunc      ready_func,
				   gpointer       user_data,
				   GError        *error);

/* debug */

void debug       (const char *file,
		  int         line,
		  const char *function,
		  const char *format,
		  ...);
void performance (const char *file,
		  int         line,
		  const char *function,
		  const char *format,
		  ...);

#define DEBUG_INFO __FILE__, __LINE__, G_STRFUNC

/* GTimeVal utils */

int             _g_time_val_cmp                  (GTimeVal   *a,
	 					  GTimeVal   *b);
void            _g_time_val_reset                (GTimeVal   *time_);
gboolean        _g_time_val_from_exif_date       (const char *exif_date,
						  GTimeVal   *time_);
char *          _g_time_val_to_exif_date         (GTimeVal   *time_);

/* Bookmark file utils */

void            _g_bookmark_file_clear           (GBookmarkFile  *bookmark);
void            _g_bookmark_file_add_uri         (GBookmarkFile  *bookmark,
						  const char     *uri);
void            _g_bookmark_file_set_uris        (GBookmarkFile  *bookmark,
						  GList          *uri_list);

/* String utils */

void            _g_strset                        (char       **s,
						  const char  *value);
char *          _g_strdup_with_max_size          (const char  *s,
						  int          max_size);
char **         _g_get_template_from_text        (const char  *s_template);
char *          _g_get_name_from_template        (char       **s_template,
						  int          num);
char *          _g_replace                       (const char  *str,
						  const char  *from_str,
						  const char  *to_str);
char *          _g_replace_pattern               (const char  *utf8_text,
						  gunichar     pattern,
						  const char  *value);
char *          _g_utf8_replace                  (const char  *string,
						  const char  *pattern,
						  const char  *replacement);
char *          _g_utf8_strndup                  (const char  *str,
						  gsize        n);
char **         _g_utf8_strsplit                 (const char *string,
						  const char *delimiter,
						  int         max_tokens);
char *          _g_utf8_strstrip                 (const char  *str);
gboolean        _g_utf8_all_spaces               (const char  *utf8_string);
GList *         _g_list_insert_list_before       (GList       *list1,
						  GList       *sibling,
						  GList       *list2);
const char *    get_static_string                (const char  *s);
char *          _g_rand_string                   (int          len);
int             _g_strv_find                     (char        **v,
						  const char   *s);
char *          _g_str_remove_suffix             (const char   *s,
						  const char   *suffix);

/* Regexp utils */

GRegex **       get_regexps_from_pattern         (const char  *pattern_string,
						  GRegexCompileFlags  compile_options);
gboolean        string_matches_regexps           (GRegex     **regexps,
						  const char  *string,
						  GRegexMatchFlags match_options);
void            free_regexps                     (GRegex     **regexps);


/* URI utils */

const char *    get_home_uri                     (void);
const char *    get_desktop_uri                  (void);
char *          xdg_user_dir_lookup              (const char *type);
int             uricmp                           (const char *uri1,
						  const char *uri2);
gboolean        same_uri                         (const char *uri1,
						  const char *uri2);
void            _g_string_list_free              (GList      *string_list);
GList *         _g_string_list_dup               (GList      *string_list);
GType           g_string_list_get_type           (void);
GList *         get_file_list_from_url_list      (char       *url_list);
const char *    _g_uri_get_basename              (const char *uri);
const char *    _g_uri_get_file_extension        (const char *uri);
gboolean        _g_uri_is_file                   (const char *uri);
gboolean        _g_uri_is_dir                    (const char *uri);
gboolean        _g_uri_parent_of_uri             (const char *dir,
						  const char *file);
char *          _g_uri_get_parent                (const char *uri);
char *          _g_uri_remove_extension          (const char *uri);
char *          _g_build_uri                     (const char *base,
						  ...);

/* GIO utils */

gboolean        _g_file_equal                    (GFile      *file1,
						  GFile      *file2);
char *          _g_file_get_display_name         (GFile      *file);
GFileType 	_g_file_get_standard_type        (GFile      *file);
GFile *         _g_file_get_destination          (GFile      *source,
						  GFile      *source_base,
						  GFile      *destination_folder);
GFile *         _g_file_get_child                (GFile      *file,
						  ...);
GIcon *         _g_file_get_icon                 (GFile      *file);
GList *         _g_file_list_dup                 (GList      *l);
void            _g_file_list_free                (GList      *l);
GList *         _g_file_list_new_from_uri_list   (GList      *uris);
GList *         _g_file_list_new_from_uriv       (char      **uris);
GList *         _g_file_list_find_file           (GList      *l,
						  GFile      *file);
const char*     _g_file_get_mime_type            (GFile      *file,
						  gboolean    fast_file_type);
void            _g_file_get_modification_time    (GFile      *file,
						  GTimeVal   *result);
time_t          _g_file_get_mtime                (GFile      *file);
int             _g_file_cmp_uris                 (GFile      *a,
						  GFile      *b);
int             _g_file_cmp_modification_time    (GFile      *a,
						  GFile      *b);
goffset         _g_file_get_size                 (GFile      *info);
GFile *         _g_file_resolve_all_symlinks     (GFile      *file,
						  GError    **error);
GFile *         _g_file_append_prefix            (GFile      *file,
						  const char *prefix);
GFile *         _g_file_append_path              (GFile      *file,
						  const char *path);
gboolean        _g_file_attributes_matches       (const char *attributes,
						  const char *mask);
void            _g_file_info_swap_attributes     (GFileInfo  *info,
						  const char *attr1,
						  const char *attr2);
gboolean        _g_mime_type_is_image            (const char *mime_type);
gboolean        _g_mime_type_is_video            (const char *mime_type);
gboolean        _g_mime_type_is_audio            (const char *mime_type);

G_END_DECLS

#endif /* _GLIB_UTILS_H */