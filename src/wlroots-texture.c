/* wlroots-texture.c
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

#include "wlroots-texture.h"

struct _WlrootsTexture
{
  GObject parent_instance;

  struct wlr_texture *wrapped_texture;
};

G_DEFINE_TYPE (WlrootsTexture, wlroots_texture, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WLROOTS_TEXTURE,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

WlrootsTexture *
wlroots_texture_new (void)
{
  return g_object_new (WLROOTS_TYPE_TEXTURE, NULL);
}

WlrootsTexture *
wlroots_texture_wrap (struct wlr_texture *texture)
{
  return g_object_new (WLROOTS_TYPE_TEXTURE, "wlroots-texture", texture, NULL);
}

struct wlr_texture *
wlroots_texture_get_wlr_texture (WlrootsTexture *self)
{
  return self->wrapped_texture;
}

static void
wlroots_texture_finalize (GObject *object)
{
  WlrootsTexture *self = (WlrootsTexture *)object;

  G_OBJECT_CLASS (wlroots_texture_parent_class)->finalize (object);
}

static void
wlroots_texture_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  WlrootsTexture *self = WLROOTS_TEXTURE (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_TEXTURE:
      g_value_set_pointer (value, self->wrapped_texture);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_texture_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  WlrootsTexture *self = WLROOTS_TEXTURE (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_TEXTURE:
      self->wrapped_texture = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_texture_class_init (WlrootsTextureClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_texture_finalize;
  object_class->get_property = wlroots_texture_get_property;
  object_class->set_property = wlroots_texture_set_property;

  properties [PROP_WLROOTS_TEXTURE] =
    g_param_spec_pointer ("wlroots-texture",
                          "WlrootsTexture",
                          "WlrootsTexture",

                          (G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS));
  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
wlroots_texture_init (WlrootsTexture *self)
{
}
