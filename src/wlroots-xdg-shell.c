/* wlroots-xdg-shell.c
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

#include "wlroots-xdg-shell.h"
#include "wlroots-xdg-surface.h"
#include <wlr/types/wlr_xdg_shell.h>
#include <wayland-server-core.h>

struct _WlrootsXDGShell
{
  GObject parent_instance;

  struct wlr_xdg_shell *wrapped_xdg_shell;
  struct wl_display *wayland_display;

  struct wl_listener new_surface;
};

G_DEFINE_TYPE (WlrootsXDGShell, wlroots_xdg_shell, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WAYLAND_DISPLAY,
  N_PROPS
};

enum {
  NEW_SURFACE,
  N_SIGNALS
};

static GParamSpec *properties [N_PROPS];
static guint signals[N_SIGNALS];

WlrootsXDGShell *
wlroots_xdg_shell_new (void)
{
  return g_object_new (WLROOTS_TYPE_XDG_SHELL, NULL);
}

/**
 * wlroots_xdg_shell_create: (constructor):
 * @display: (transfer none): A #WlrootsWaylandDisplay to create this shell from
 *
 * Returns: (transfer full): A new #WlrootsXDGShell instance
 *
 * Since: 0.1
 */
WlrootsXDGShell *
wlroots_xdg_shell_create (WlrootsWaylandDisplay *display)
{
  struct wl_display *way_display = wlroots_wayland_display_get_wl_display (display);
  return g_object_new (WLROOTS_TYPE_XDG_SHELL, "wayland-display", way_display, NULL);
}

static void
wlroots_xdg_shell_finalize (GObject *object)
{
  WlrootsXDGShell *self = (WlrootsXDGShell *)object;

  G_OBJECT_CLASS (wlroots_xdg_shell_parent_class)->finalize (object);
}

static void
wlroots_xdg_shell_get_property (GObject    *object,
                                guint       prop_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
  WlrootsXDGShell *self = WLROOTS_XDG_SHELL (object);

  switch (prop_id)
    {
    case PROP_WAYLAND_DISPLAY:
      g_value_set_pointer (value, self->wayland_display);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_xdg_shell_set_property (GObject      *object,
                                guint         prop_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
  WlrootsXDGShell *self = WLROOTS_XDG_SHELL (object);

  switch (prop_id)
    {
    case PROP_WAYLAND_DISPLAY:
      self->wayland_display = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
xdg_shell_new_surface (struct wl_listener *listener, void* data)
{
  WlrootsXDGShell *self = wl_container_of (listener, self, new_surface);
  g_signal_emit (self, signals[NEW_SURFACE], 0);
}

static void
wlroots_xdg_shell_constructed (GObject *obj)
{
  WlrootsXDGShell *self = WLROOTS_XDG_SHELL (obj);

  self->wrapped_xdg_shell = wlr_xdg_shell_create (self->wayland_display);
  self->new_surface.notify = xdg_shell_new_surface;
  wl_signal_add (&self->wrapped_xdg_shell->events.new_surface, &self->new_surface);
}

static void
wlroots_xdg_shell_class_init (WlrootsXDGShellClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_xdg_shell_finalize;
  object_class->get_property = wlroots_xdg_shell_get_property;
  object_class->set_property = wlroots_xdg_shell_set_property;
  object_class->constructed = wlroots_xdg_shell_constructed;

  properties [PROP_WAYLAND_DISPLAY] =
    g_param_spec_pointer ("wayland-display",
                          "WaylandDisplay",
                          "WaylandDisplay",

                          (G_PARAM_WRITABLE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS));
  g_object_class_install_properties (object_class, N_PROPS, properties);

  signals [NEW_SURFACE] =
    g_signal_new ("new-surface",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  1,
                  WLROOTS_TYPE_XDG_SURFACE);
}

static void
wlroots_xdg_shell_init (WlrootsXDGShell *self)
{
}
