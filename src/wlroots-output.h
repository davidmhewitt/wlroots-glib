/* wlroots-output.h
 *
 * Copyright 2019 David Hewitt <davidmhewitt@gmail.com>
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include <glib-object.h>
#include <wlr/types/wlr_output.h>

G_BEGIN_DECLS

#define WLROOTS_TYPE_OUTPUT (wlroots_output_get_type())

G_DECLARE_FINAL_TYPE (WlrootsOutput, wlroots_output, WLROOTS, OUTPUT, GObject)

typedef struct _WlrootsOutputMode WlrootsOutputMode;

WlrootsOutput *wlroots_output_wrap (struct wlr_output *output);
GList *wlroots_output_get_modes (WlrootsOutput *self);
void wlroots_output_set_mode (WlrootsOutput *self, WlrootsOutputMode *mode);
struct wlr_output *wlroots_output_get_wlr_output (WlrootsOutput *self);
void wlroots_output_create_global (WlrootsOutput *self);
gboolean wlroots_output_attach_render (WlrootsOutput *self, int *buffer_age);
void wlroots_output_effective_resolution (WlrootsOutput *self, int *width, int *height);
void wlroots_output_commit (WlrootsOutput *self);
void wlroots_output_render_software_cursors (WlrootsOutput *self);

/**
 * WlrootsOutputMode:
 * @ref_count: the reference count of the struct.
 * @width: the width of the mode.
 * @height: the height of the mode.
 * @preferred: whether this mode is preferred.
 *
 * #WlrootsOutputMode specifies an available output mode.
 */
struct _WlrootsOutputMode
{
  struct wlr_output_mode *wlr_mode;
  /*< public >*/
  int ref_count;
  gint32 width;
  gint32 height;
  gint32 refresh;
};

#define WLROOTS_TYPE_OUTPUT_MODE  (wlroots_output_mode_get_type ())
GType              wlroots_output_mode_get_type (void) G_GNUC_CONST;

WlrootsOutputMode *wlroots_output_mode_new (struct wlr_output_mode *wlr_mode, gint32 width, gint32 height, gint32 refresh);

G_END_DECLS
