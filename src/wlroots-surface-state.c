/* wlroots-surface-state.c
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

#include "wlroots-surface-state.h"
#include "wlroots-output-transform.h"
#include "wlroots-enum-types.h"

struct _WlrootsSurfaceState
{
  GObject parent_instance;

  struct wlr_surface_state *wrapped_surface_state;
};

G_DEFINE_TYPE (WlrootsSurfaceState, wlroots_surface_state, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WLROOTS_SURFACE_STATE,
  PROP_WIDTH,
  PROP_HEIGHT,
  PROP_TRANSFORM,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

WlrootsSurfaceState *
wlroots_surface_state_new (void)
{
  return g_object_new (WLROOTS_TYPE_SURFACE_STATE, NULL);
}

/**
 * wlroots_surface_state_wrap:
 * @state: (transfer none): The surface state to wrap
 *
 * Returns: (transfer full): A new #WlrootsSurfaceState instance
 *
 * Since: 0.1
 */
WlrootsSurfaceState *
wlroots_surface_state_wrap (struct wlr_surface_state *state)
{
  return g_object_new (WLROOTS_TYPE_SURFACE_STATE, "wlroots-surface-state", state, NULL);
}


static void
wlroots_surface_state_finalize (GObject *object)
{
  WlrootsSurfaceState *self = (WlrootsSurfaceState *)object;

  G_OBJECT_CLASS (wlroots_surface_state_parent_class)->finalize (object);
}

static void
wlroots_surface_state_get_property (GObject    *object,
                                    guint       prop_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
  WlrootsSurfaceState *self = WLROOTS_SURFACE_STATE (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_SURFACE_STATE:
      g_value_set_pointer (value, self->wrapped_surface_state);
      break;
    case PROP_WIDTH:
      g_value_set_int (value, self->wrapped_surface_state->width);
      break;
    case PROP_HEIGHT:
      g_value_set_int (value, self->wrapped_surface_state->height);
      break;
    case PROP_TRANSFORM:
      g_value_set_enum (value, self->wrapped_surface_state->transform);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_surface_state_set_property (GObject      *object,
                                    guint         prop_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
  WlrootsSurfaceState *self = WLROOTS_SURFACE_STATE (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_SURFACE_STATE:
      self->wrapped_surface_state = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_surface_state_class_init (WlrootsSurfaceStateClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_surface_state_finalize;
  object_class->get_property = wlroots_surface_state_get_property;
  object_class->set_property = wlroots_surface_state_set_property;

  properties [PROP_WLROOTS_SURFACE_STATE] =
    g_param_spec_pointer ("wlroots-surface-state",
                          "WlrootsSurfaceState",
                          "WlrootsSurfaceState",

                          (G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS));

  properties [PROP_WIDTH] =
    g_param_spec_int ("width",
                      "Width",
                      "Width",
                      G_MININT,
                      G_MAXINT,
                      0,
                      (G_PARAM_READABLE |
                       G_PARAM_STATIC_STRINGS));

  properties [PROP_HEIGHT] =
    g_param_spec_int ("height",
                      "Height",
                      "Height",
                      G_MININT,
                      G_MAXINT,
                      0,
                      (G_PARAM_READABLE |
                       G_PARAM_STATIC_STRINGS));

  /**
   * WlrootsSurfaceState:transform: (type WlrootsWaylandOutputTransform)
   *
   * The transformation.
   */
  properties [PROP_TRANSFORM] =
    g_param_spec_enum ("transform",
                       "Transform",
                       "Transform",
                       WLROOTS_TYPE_WAYLAND_OUTPUT_TRANSFORM,
                       WLROOTS_WAYLAND_OUTPUT_TRANSFORM_NORMAL,
                       (G_PARAM_READABLE |
                        G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
wlroots_surface_state_init (WlrootsSurfaceState *self)
{
}
