/* wlroots-compositor.c
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

#include "wlroots-compositor.h"
#include <wlr/types/wlr_compositor.h>

struct _WlrootsCompositor
{
  GObject parent_instance;

  struct wlr_compositor *wrapped_compositor;
};

G_DEFINE_TYPE (WlrootsCompositor, wlroots_compositor, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WLROOTS_COMPOSITOR,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

WlrootsCompositor *
wlroots_compositor_new (void)
{
  return g_object_new (WLROOTS_TYPE_COMPOSITOR, NULL);
}

/**
 * wlroots_compositor_create:
 * @display: (transfer none): A valid #WlrootsWaylandDisplay to create the compositor from
 * @renderer: (transfer none): A valid #WlrootsRenderer to create the compositor from
 *
 * Returns: (transfer full): A new #WlrootsCompositor instance
 *
 * Since: 0.1
 */
WlrootsCompositor *
wlroots_compositor_create (WlrootsWaylandDisplay *display, WlrootsRenderer *renderer)
{
  struct wl_display *way_display = wlroots_wayland_display_get_wl_display (display);
  struct wlr_renderer *roots_renderer = wlroots_renderer_get_wlr_renderer (renderer);

  struct wlr_compositor *compositor = wlr_compositor_create (way_display, roots_renderer);
  return g_object_new (WLROOTS_TYPE_COMPOSITOR, "wlroots-compositor", compositor, NULL);
}

static void
wlroots_compositor_get_property (GObject    *object,
                                 guint       prop_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
  WlrootsCompositor *self = WLROOTS_COMPOSITOR (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_COMPOSITOR:
      g_value_set_pointer (value, self->wrapped_compositor);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_compositor_set_property (GObject      *object,
                                 guint         prop_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
  WlrootsCompositor *self = WLROOTS_COMPOSITOR (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_COMPOSITOR:
      self->wrapped_compositor = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_compositor_class_init (WlrootsCompositorClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = wlroots_compositor_get_property;
  object_class->set_property = wlroots_compositor_set_property;

  properties [PROP_WLROOTS_COMPOSITOR] =
    g_param_spec_pointer ("wlroots-compositor",
                          "WlrootsCompositor",
                          "WlrootsCompositor",
                          G_PARAM_CONSTRUCT_ONLY |
                          G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS);
  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
wlroots_compositor_init (WlrootsCompositor *self)
{
}

