/* wlroots-renderer.c
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

#include "wlroots-renderer.h"

struct _WlrootsRenderer
{
  GObject parent_instance;

  struct wlr_renderer *wrapped_renderer;
};

G_DEFINE_TYPE (WlrootsRenderer, wlroots_renderer, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WLR_RENDERER,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

WlrootsRenderer *
wlroots_renderer_new (void)
{
  return g_object_new (WLROOTS_TYPE_RENDERER, NULL);
}

/**
 * wlroots_renderer_wrap: (constructor):
 * @renderer: (transfer none): The #wlr_renderer instance to wrap
 *
 * Returns: (transfer full): A new #WlrootsRenderer instance
 *
 * Since: 0.1
 */
WlrootsRenderer *
wlroots_renderer_wrap (struct wlr_renderer *renderer)
{
  return g_object_new (WLROOTS_TYPE_RENDERER, "wlr-renderer", renderer, NULL);
}

void
wlroots_renderer_init_wl_display (WlrootsRenderer *self, WlrootsWaylandDisplay *display)
{
  struct wl_display *way_display = wlroots_wayland_display_get_wl_display (display);
  wlr_renderer_init_wl_display (self->wrapped_renderer, way_display);
}

struct wlr_renderer *
wlroots_renderer_get_wlr_renderer (WlrootsRenderer *self)
{
  return self->wrapped_renderer;
}

void
wlroots_renderer_begin (WlrootsRenderer* self, int width, int height)
{
  wlr_renderer_begin (self->wrapped_renderer, width, height);
}

void
wlroots_renderer_end (WlrootsRenderer * self)
{
  wlr_renderer_end (self->wrapped_renderer);
}

/**
 * wlroots_renderer_clear:
 * @color: (array fixed-size=4): Array of 4 floats, RGBA color to clear
 *
 * Since: 0.2
 */
void
wlroots_renderer_clear (WlrootsRenderer *self, const float color[])
{
  wlr_renderer_clear (self->wrapped_renderer, color);
}

static void
wlroots_renderer_get_property (GObject    *object,
                               guint       prop_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
  WlrootsRenderer *self = WLROOTS_RENDERER (object);

  switch (prop_id)
    {
    case PROP_WLR_RENDERER:
      g_value_set_pointer (value, wlroots_renderer_get_wlr_renderer (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_renderer_set_property (GObject      *object,
                               guint         prop_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
  WlrootsRenderer *self = WLROOTS_RENDERER (object);

  switch (prop_id)
    {
    case PROP_WLR_RENDERER:
      self->wrapped_renderer = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_renderer_class_init (WlrootsRendererClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = wlroots_renderer_get_property;
  object_class->set_property = wlroots_renderer_set_property;

  properties [PROP_WLR_RENDERER] =
    g_param_spec_pointer ("wlr-renderer",
                         "WlrRenderer",
                         "WlrRenderer",
                          G_PARAM_READWRITE |
                          G_PARAM_CONSTRUCT_ONLY |
                          G_PARAM_STATIC_STRINGS);
  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
wlroots_renderer_init (WlrootsRenderer *self)
{
}
