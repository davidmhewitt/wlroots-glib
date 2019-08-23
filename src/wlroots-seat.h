/* wlroots-seat.h
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
#include "wlroots-wayland-display.h"
#include "wlroots-events.h"
#include "wlroots-surface.h"

G_BEGIN_DECLS

#define WLROOTS_TYPE_SEAT (wlroots_seat_get_type())

G_DECLARE_FINAL_TYPE (WlrootsSeat, wlroots_seat, WLROOTS, SEAT, GObject)

WlrootsSeat *wlroots_seat_new (WlrootsWaylandDisplay *display, gchar *name);

typedef enum {
  WLROOTS_WAYLAND_SEAT_CAPABILITY_POINTER = 1 << 1,
  WLROOTS_WAYLAND_SEAT_CAPABILITY_KEYBOARD = 1 << 2,
  WLROOTS_WAYLAND_SEAT_CAPABILITY_TOUCH = 1 << 3
} WlrootsWaylandSeatCapability;

void wlroots_seat_set_capabilities (WlrootsSeat *self, WlrootsWaylandSeatCapability caps);
void wlroots_seat_pointer_notify_axis (WlrootsSeat *self,
                                       guint32 msec,
                                       WlrootsEventAxisOrientation orientation,
                                       gdouble delta,
                                       gint32 delta_discrete,
                                       WlrootsEventAxisSource source);
void wlroots_seat_pointer_notify_button (WlrootsSeat *self,
                                         guint32 msec,
                                         guint32 button,
                                         WlrootsButtonState state);
void wlroots_seat_pointer_notify_enter (WlrootsSeat *self,
                                        WlrootsSurface *surface,
                                        gdouble sx,
                                        gdouble sy);
void wlroots_seat_pointer_clear_focus (WlrootsSeat *self);
void wlroots_seat_pointer_notify_motion (WlrootsSeat *self,
                                         guint32 msec,
                                         gdouble x,
                                         gdouble y);

G_END_DECLS
