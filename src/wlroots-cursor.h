/* wlroots-cursor.h
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
#include "wlroots-output-layout.h"
#include "wlroots-input-device.h"

G_BEGIN_DECLS

#define WLROOTS_TYPE_CURSOR (wlroots_cursor_get_type())

G_DECLARE_FINAL_TYPE (WlrootsCursor, wlroots_cursor, WLROOTS, CURSOR, GObject)

WlrootsCursor *wlroots_cursor_new (void);
void wlroots_cursor_attach_output_layout (WlrootsCursor *self, WlrootsOutputLayout *output_layout);
void wlroots_cursor_attach_input_device (WlrootsCursor *self, WlrootsInputDevice *input_dev);
void wlroots_cursor_warp_absolute (WlrootsCursor *self, WlrootsInputDevice *device, gdouble x, gdouble y);
struct wl_cursor *wlroots_cursor_get_wlr_cursor (WlrootsCursor *self);

G_END_DECLS
