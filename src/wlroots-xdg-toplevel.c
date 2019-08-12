/* wlroots-xdg-toplevel.c
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

#include "wlroots-xdg-toplevel.h"

struct _WlrootsXDGToplevel
{
  GObject parent_instance;

  struct wlr_xdg_toplevel *wrapped_toplevel;

  struct wl_listener request_move;
  struct wl_listener request_resize;
};

G_DEFINE_TYPE (WlrootsXDGToplevel, wlroots_xdg_toplevel, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_XDG_TOPLEVEL,
  N_PROPS
};

enum {
  REQUEST_MOVE,
  REQUEST_RESIZE,
  N_SIGNALS
};

static GParamSpec *properties [N_PROPS];
static gint signals[N_SIGNALS];

WlrootsXDGToplevel *
wlroots_xdg_toplevel_new (void)
{
  return g_object_new (WLROOTS_TYPE_XDG_TOPLEVEL, NULL);
}

/**
 * wlroots_xdg_toplevel_wrap:
 * @toplevel: (transfer none): The XDG toplevel struct pointer to wrap
 *
 * Returns: (transfer full): A new #WlrootsXDGToplevel instance
 *
 * Since: 0.1
 */
WlrootsXDGToplevel *
wlroots_xdg_toplevel_wrap (struct wlr_xdg_toplevel *toplevel)
{
  return g_object_new (WLROOTS_TYPE_XDG_TOPLEVEL, "xdg-toplevel", toplevel, NULL);
}

gboolean
wlroots_xdg_toplevel_equal (WlrootsXDGToplevel *a, WlrootsXDGToplevel *b)
{
  return a->wrapped_toplevel == b->wrapped_toplevel;
}

static void
wlroots_xdg_toplevel_finalize (GObject *object)
{
  WlrootsXDGToplevel *self = (WlrootsXDGToplevel *)object;

  G_OBJECT_CLASS (wlroots_xdg_toplevel_parent_class)->finalize (object);
}

static void
wlroots_xdg_toplevel_get_property (GObject    *object,
                                   guint       prop_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
  WlrootsXDGToplevel *self = WLROOTS_XDG_TOPLEVEL (object);

  switch (prop_id)
    {
    case PROP_XDG_TOPLEVEL:
      g_value_set_pointer (value, self->wrapped_toplevel);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_xdg_toplevel_set_property (GObject      *object,
                                   guint         prop_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
  WlrootsXDGToplevel *self = WLROOTS_XDG_TOPLEVEL (object);

  switch (prop_id)
    {
    case PROP_XDG_TOPLEVEL:
      self->wrapped_toplevel = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
xdg_toplevel_request_move (struct wl_listener *listener, void *data)
{
  WlrootsXDGToplevel *self = wl_container_of (listener, self, request_move);

  g_signal_emit (self, signals[REQUEST_MOVE], 0);
}

static void
xdg_toplevel_request_resize (struct wl_listener *listener, void *data)
{
  WlrootsXDGToplevel *self = wl_container_of (listener, self, request_resize);

  g_signal_emit (self, signals[REQUEST_RESIZE], 0);
}

static void
wlroots_xdg_toplevel_constructed (GObject *obj)
{
  WlrootsXDGToplevel *self = WLROOTS_XDG_TOPLEVEL (obj);

  self->request_move.notify = xdg_toplevel_request_move;
  self->request_resize.notify = xdg_toplevel_request_resize;

  wl_signal_add (&self->wrapped_toplevel->events.request_move, &self->request_move);
  wl_signal_add (&self->wrapped_toplevel->events.request_resize, &self->request_move);

  G_OBJECT_CLASS(wlroots_xdg_toplevel_parent_class)->constructed (obj);
}

static void
wlroots_xdg_toplevel_class_init (WlrootsXDGToplevelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_xdg_toplevel_finalize;
  object_class->get_property = wlroots_xdg_toplevel_get_property;
  object_class->set_property = wlroots_xdg_toplevel_set_property;
  object_class->constructed = wlroots_xdg_toplevel_constructed;

  properties [PROP_XDG_TOPLEVEL] =
    g_param_spec_pointer ("xdg-toplevel",
                          "XdgToplevel",
                          "XdgToplevel",

                          (G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS));
  g_object_class_install_properties (object_class, N_PROPS, properties);

  signals [REQUEST_MOVE] =
    g_signal_new ("request-move",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  0);

  signals [REQUEST_RESIZE] =
    g_signal_new ("request-resize",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  0);
}

static void
wlroots_xdg_toplevel_init (WlrootsXDGToplevel *self)
{
}
