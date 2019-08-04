/* wlroots-wayland-display.c
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

#include "wlroots-wayland-display.h"
#include <wayland-server-core.h>

struct _WlrootsWaylandDisplay
{
  GObject parent_instance;

  struct wl_display *wrapped_display;
};

G_DEFINE_TYPE (WlrootsWaylandDisplay, wlroots_wayland_display, G_TYPE_OBJECT)

WlrootsWaylandDisplay *
wlroots_wayland_display_new (void)
{
  return g_object_new (WLROOTS_TYPE_WAYLAND_DISPLAY, NULL);
}

static void
wlroots_wayland_display_finalize (GObject *object)
{
  WlrootsWaylandDisplay *self = (WlrootsWaylandDisplay *)object;

  wl_display_destroy_clients (self->wrapped_display);
  wl_display_destroy (self->wrapped_display);

  G_OBJECT_CLASS (wlroots_wayland_display_parent_class)->finalize (object);
}

struct wl_display *
wlroots_wayland_display_get_wl_display (WlrootsWaylandDisplay *self)
{
  return self->wrapped_display;
}

gchar *
wlroots_wayland_display_add_socket_auto (WlrootsWaylandDisplay *self)
{
  const gchar *socket = wl_display_add_socket_auto (self->wrapped_display);
  return g_strdup (socket);
}

void
wlroots_wayland_display_run (WlrootsWaylandDisplay *self)
{
  wl_display_run (self->wrapped_display);
}

static void
wlroots_wayland_display_class_init (WlrootsWaylandDisplayClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_wayland_display_finalize;
}

static void
wlroots_wayland_display_init (WlrootsWaylandDisplay *self)
{
  self->wrapped_display = wl_display_create ();
}
