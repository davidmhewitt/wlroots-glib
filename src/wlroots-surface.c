/* wlroots-surface.c
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

#include <wlr/types/wlr_surface.h>
#include "wlroots-surface.h"
#include "wlroots-surface-state.h"
#include "pointer-table.h"

struct _WlrootsSurface
{
  GObject parent_instance;

  struct wlr_surface *wrapped_surface;

  struct wl_listener destroy;
};

G_DEFINE_TYPE (WlrootsSurface, wlroots_surface, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WLROOTS_SURFACE,
  PROP_CURRENT,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

WlrootsSurface *
wlroots_surface_new (void)
{
  return g_object_new (WLROOTS_TYPE_SURFACE, NULL);
}

WlrootsSurface *
wlroots_surface_wrap (struct wlr_surface *surface)
{
  WlrootsSurface *existing_surface = lookup_pointer (surface);

  if (existing_surface)
  {
    return existing_surface;
  }

  existing_surface = g_object_new (WLROOTS_TYPE_SURFACE, "wlroots-surface", surface, NULL);
  g_hash_table_insert (wlroots_pointers, surface, existing_surface);

  return existing_surface;
}

/**
 * wlroots_surface_get_texture:
 *
 * Returns: (transfer full): A new #WlrootsTexture
 *
 * Since: 0.1
 */
WlrootsTexture *
wlroots_surface_get_texture (WlrootsSurface *self)
{
  struct wlr_texture *texture = wlr_surface_get_texture (self->wrapped_surface);
  if (!texture) {
    return NULL;
  }

  return wlroots_texture_wrap (texture);
}

void
wlroots_surface_send_frame_done (WlrootsSurface *self, struct timespec *when)
{
  wlr_surface_send_frame_done (self->wrapped_surface, when);
}

struct wlr_surface *
wlroots_surface_get_wlr_surface (WlrootsSurface *self)
{
  return self->wrapped_surface;
}

static void
wlroots_surface_finalize (GObject *object)
{
  WlrootsSurface *self = (WlrootsSurface *)object;

  G_OBJECT_CLASS (wlroots_surface_parent_class)->finalize (object);
}

static void
wlroots_surface_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  WlrootsSurface *self = WLROOTS_SURFACE (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_SURFACE:
      g_value_set_pointer (value, self->wrapped_surface);
      break;
    case PROP_CURRENT:
      g_value_set_object (value, wlroots_surface_state_wrap (&self->wrapped_surface->current));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_surface_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  WlrootsSurface *self = WLROOTS_SURFACE (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_SURFACE:
      self->wrapped_surface = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
surface_destroy (struct wl_listener *listener, void* data)
{
  WlrootsSurface *self = wl_container_of (listener, self, destroy);

  g_hash_table_remove (wlroots_pointers, self->wrapped_surface);
}

static void
wlroots_surface_constructed (GObject *obj)
{
  WlrootsSurface *self = WLROOTS_SURFACE (obj);

  self->destroy.notify = surface_destroy;

  wl_signal_add (&self->wrapped_surface->events.destroy, &self->destroy);

  G_OBJECT_CLASS(wlroots_surface_parent_class)->constructed (obj);
}

static void
wlroots_surface_class_init (WlrootsSurfaceClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_surface_finalize;
  object_class->get_property = wlroots_surface_get_property;
  object_class->set_property = wlroots_surface_set_property;
  object_class->constructed = wlroots_surface_constructed;

  properties [PROP_WLROOTS_SURFACE] =
    g_param_spec_pointer ("wlroots-surface",
                          "WlrootsSurface",
                          "WlrootsSurface",

                          (G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS));

  properties [PROP_CURRENT] =
    g_param_spec_object ("current",
                         "Current",
                         "Current",
                         WLROOTS_TYPE_SURFACE_STATE,
                         (G_PARAM_READABLE |
                          G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
wlroots_surface_init (WlrootsSurface *self)
{
}
