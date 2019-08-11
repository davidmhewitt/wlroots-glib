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

struct _WlrootsXDGSurface
{
  GObject parent_instance;

  struct wlr_xdg_surface *wrapped_surface;
};

G_DEFINE_TYPE (WlrootsXDGSurface, wlroots_xdg_surface, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_XDG_SURFACE,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

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
  return g_object_new (WLROOTS_TYPE_XDG_SURFACE, "wrapped-surface", surface, NULL);
}

static void
wlroots_xdg_surface_finalize (GObject *object)
{
  WlrootsXDGSurface *self = (WlrootsXDGSurface *)object;

  G_OBJECT_CLASS (wlroots_xdg_surface_parent_class)->finalize (object);
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
wlroots_xdg_surface_constructed (GObject *obj)
{
  WlrootsXDGSurface *self = WLROOTS_XDG_SURFACE (obj);

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
  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
wlroots_xdg_surface_init (WlrootsXDGSurface *self)
{
}
