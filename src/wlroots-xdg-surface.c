/* wlroots-xdg-surface.c
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

#include "wlroots-xdg-surface.h"
#include "wlroots-enum-types.h"

struct _WlrootsXDGSurface
{
  GObject parent_instance;

  struct wlr_xdg_surface *wrapped_surface;

  struct wl_listener map;
  struct wl_listener unmap;
  struct wl_listener destroy;

  WlrootsXDGToplevel *toplevel;
};

G_DEFINE_TYPE (WlrootsXDGSurface, wlroots_xdg_surface, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_XDG_SURFACE,
  PROP_ROLE,
  PROP_TOPLEVEL,
  N_PROPS
};

enum {
  MAP,
  UNMAP,
  DESTROY,
  N_SIGNALS
};

static GParamSpec *properties [N_PROPS];
static gint signals[N_SIGNALS];

WlrootsXDGSurface *
wlroots_xdg_surface_new (void)
{
  return g_object_new (WLROOTS_TYPE_XDG_SURFACE, NULL);
}

/**
 * wlroots_xdg_surface_wrap:
 * @surface: (transfer none): The XDG Surface to wrap
 *
 * Returns: (transfer full): A new #WlrootsXDGSurface instance
 *
 * Since: 0.1
 */
WlrootsXDGSurface *
wlroots_xdg_surface_wrap (struct wlr_xdg_surface *surface)
{
  return g_object_new (WLROOTS_TYPE_XDG_SURFACE, "xdg-surface", surface, NULL);
}

/**
 * wlroots_xdg_surface_get_geometry:
 * @box: (out) (transfer full): A #WlrootsBox containing the geometry of the surface
 *
 * Since: 0.1
 */
void
wlroots_xdg_surface_get_geometry (WlrootsXDGSurface *self, WlrootsBox **box)
{
  struct wlr_box geometry;
  wlr_xdg_surface_get_geometry (self->wrapped_surface, &geometry);
  *box = wlroots_box_wrap (&geometry);
}

struct foreach_data {
	WlrootsXDGSurfaceFunc func;
	gpointer user_data;
};

static void
xdg_surface_for_each (struct wlr_surface *surface, int sx, int sy, void *data)
{
  struct foreach_data *callback_data = data;
  WlrootsXDGSurfaceFunc func = callback_data->func;
  gpointer user_data = callback_data->user_data;

  WlrootsSurface* wrapped_surface = wlroots_surface_wrap (surface);

  (*func)(wrapped_surface, sx, sy, user_data);

  g_object_unref (wrapped_surface);
}

/**
 * wlroots_xdg_surface_for_each_surface:
 * @func: (scope call): The #XDGSurfaceFunc to be applied to each surface
 * @user_data: user data passed to @func at each call
 *
 * Since: 0.1
 */
void
wlroots_xdg_surface_for_each_surface (WlrootsXDGSurface *self, WlrootsXDGSurfaceFunc func, gpointer user_data)
{
  struct foreach_data callback_data = {
			.func = func,
			.user_data = user_data,
	};

  wlr_xdg_surface_for_each_surface (self->wrapped_surface, xdg_surface_for_each, &callback_data);
}

static void
wlroots_xdg_surface_finalize (GObject *object)
{
  WlrootsXDGSurface *self = (WlrootsXDGSurface *)object;

  G_OBJECT_CLASS (wlroots_xdg_surface_parent_class)->finalize (object);
}

/**
 * wlroots_xdg_surface_get_toplevel:
 *
 * Returns: (transfer none): The #WlrootsXDGToplevel instance associated with this surface
 *
 * Since: 0.1
 */
WlrootsXDGToplevel *
wlroots_xdg_surface_get_toplevel (WlrootsXDGSurface *self)
{
  if (!self->toplevel) {
    self->toplevel = wlroots_xdg_toplevel_wrap (self->wrapped_surface->toplevel);
  }

  return self->toplevel;
}

/**
 * wlroots_xdg_surface_surface_at:
 * @sx: (out): The x co-ordinate of the top left of the touched surface
 * @sy: (out): The y co-ordinate of the top left of the touched surface
 *
 * Returns: (nullable) (transfer full): The #WlrootsSurface at the location or NULL
 *
 * Since: 0.1
 */
WlrootsSurface *
wlroots_xdg_surface_surface_at (WlrootsXDGSurface *self, gdouble lx, gdouble ly, gdouble *sx, gdouble *sy)
{
  struct wlr_surface *_surface;
  _surface = wlr_xdg_surface_surface_at (self->wrapped_surface, lx, ly, sx, sy);

  if (!_surface) {
    return NULL;
  }

  return wlroots_surface_wrap (_surface);
}

gboolean
wlroots_xdg_surface_equal (WlrootsXDGSurface *a, WlrootsXDGSurface *b)
{
  return a->wrapped_surface == b->wrapped_surface;
}

static gint
wlr_xdg_surface_role_to_wlroots_xdg_surface_role (gint type)
{
  gint output = WLROOTS_XDG_SURFACE_ROLE_NONE;
  switch(type)
    {
    case WLR_XDG_SURFACE_ROLE_TOPLEVEL:
      output = WLROOTS_XDG_SURFACE_ROLE_TOPLEVEL;
      break;
    case WLR_XDG_SURFACE_ROLE_POPUP:
      output = WLROOTS_XDG_SURFACE_ROLE_POPUP;
      break;
    default:
      break;
    }

  return output;
}

static void
wlroots_xdg_surface_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  WlrootsXDGSurface *self = WLROOTS_XDG_SURFACE (object);

  switch (prop_id)
    {
    case PROP_XDG_SURFACE:
      g_value_set_pointer (value, self->wrapped_surface);
      break;
    case PROP_ROLE:
      g_value_set_enum (value, wlr_xdg_surface_role_to_wlroots_xdg_surface_role (self->wrapped_surface->role));
      break;
    case PROP_TOPLEVEL:
      g_value_set_object (value, wlroots_xdg_surface_get_toplevel (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_xdg_surface_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  WlrootsXDGSurface *self = WLROOTS_XDG_SURFACE (object);

  switch (prop_id)
    {
    case PROP_XDG_SURFACE:
      self->wrapped_surface = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
xdg_surface_map (struct wl_listener *listener, void* data)
{
  WlrootsXDGSurface *self = wl_container_of (listener, self, map);

  g_signal_emit (self, signals[MAP], 0);
}

static void
xdg_surface_unmap (struct wl_listener *listener, void* data)
{
  WlrootsXDGSurface *self = wl_container_of (listener, self, unmap);

  g_signal_emit (self, signals[UNMAP], 0);
}

static void
xdg_surface_destroy (struct wl_listener *listener, void* data)
{
  WlrootsXDGSurface *self = wl_container_of (listener, self, destroy);

  if (self->toplevel)
  {
    g_object_unref (self->toplevel);
  }

  g_signal_emit (self, signals[DESTROY], 0, self);
}

static void
wlroots_xdg_surface_constructed (GObject *obj)
{
  WlrootsXDGSurface *self = WLROOTS_XDG_SURFACE (obj);

  self->map.notify = xdg_surface_map;
  self->unmap.notify = xdg_surface_unmap;
  self->destroy.notify = xdg_surface_destroy;

  wl_signal_add (&self->wrapped_surface->events.map, &self->map);
  wl_signal_add (&self->wrapped_surface->events.unmap, &self->unmap);
  wl_signal_add (&self->wrapped_surface->events.destroy, &self->destroy);

  G_OBJECT_CLASS(wlroots_xdg_surface_parent_class)->constructed (obj);
}

static void
wlroots_xdg_surface_class_init (WlrootsXDGSurfaceClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_xdg_surface_finalize;
  object_class->get_property = wlroots_xdg_surface_get_property;
  object_class->set_property = wlroots_xdg_surface_set_property;
  object_class->constructed = wlroots_xdg_surface_constructed;

  properties [PROP_XDG_SURFACE] =
    g_param_spec_pointer ("xdg-surface",
                          "XdgSurface",
                          "XdgSurface",

                          (G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS));
  /**
   * WlrootsXDGSurface:role: (type WlrootsXDGSurfaceRole)
   *
   * The device type.
   */
  properties [PROP_ROLE] =
    g_param_spec_enum ("role",
                       "Role",
                       "Role",
                       WLROOTS_TYPE_XDG_SURFACE_ROLE,
                       WLROOTS_XDG_SURFACE_ROLE_NONE,
                       (G_PARAM_READABLE |
                        G_PARAM_STATIC_STRINGS));

  properties [PROP_TOPLEVEL] =
    g_param_spec_object ("toplevel",
                         "Toplevel",
                         "Toplevel",
                         WLROOTS_TYPE_XDG_TOPLEVEL,
                         (G_PARAM_READABLE |
                          G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);

  signals [MAP] =
    g_signal_new ("map",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  0);

  signals [UNMAP] =
    g_signal_new ("unmap",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  0);

  signals [DESTROY] =
    g_signal_new ("destroy",
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
wlroots_xdg_surface_init (WlrootsXDGSurface *self)
{
}
