/* wlroots-seat-keyboard-state.c
 *
 * Copyright 2019-2021 David Hewitt <davidmhewitt@gmail.com>
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

#include "wlroots-seat-keyboard-state.h"

struct _WlrootsSeatKeyboardState
{
  GObject parent_instance;

  struct wlr_seat_keyboard_state *wrapped_state;
};

G_DEFINE_TYPE (WlrootsSeatKeyboardState, wlroots_seat_keyboard_state, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WLROOTS_SEAT_KEYBOARD_STATE,
  PROP_FOCUSED_SURFACE,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

WlrootsSeatKeyboardState *
wlroots_seat_keyboard_state_new (void)
{
  return g_object_new (WLROOTS_TYPE_SEAT_KEYBOARD_STATE, NULL);
}

WlrootsSeatKeyboardState *
wlroots_seat_keyboard_state_wrap (struct wlr_seat_keyboard_state *state)
{
  return g_object_new (WLROOTS_TYPE_SEAT_KEYBOARD_STATE, "wlroots-seat-keyboard-state", state, NULL);
}

static void
wlroots_seat_keyboard_state_finalize (GObject *object)
{
  WlrootsSeatKeyboardState *self = (WlrootsSeatKeyboardState *)object;

  G_OBJECT_CLASS (wlroots_seat_keyboard_state_parent_class)->finalize (object);
}

static void
wlroots_seat_keyboard_state_get_property (GObject    *object,
                                          guint       prop_id,
                                          GValue     *value,
                                          GParamSpec *pspec)
{
  WlrootsSeatKeyboardState *self = WLROOTS_SEAT_KEYBOARD_STATE (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_SEAT_KEYBOARD_STATE:
      g_value_set_pointer (value, self->wrapped_state);
      break;
    case PROP_FOCUSED_SURFACE:
      g_value_set_object (value, wlroots_surface_wrap (self->wrapped_state->focused_surface));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

/**
 * wlroots_seat_keyboard_state_get_focused_surface:
 *
 * Returns: (nullable) (transfer none): The focused surface
 *
 * Since: 0.1
 */
WlrootsSurface *
wlroots_seat_keyboard_state_get_focused_surface (WlrootsSeatKeyboardState *self)
{
  if (self->wrapped_state->focused_surface == NULL) {
    return NULL;
  }

  return wlroots_surface_wrap (self->wrapped_state->focused_surface);
}

static void
wlroots_seat_keyboard_state_set_property (GObject      *object,
                                          guint         prop_id,
                                          const GValue *value,
                                          GParamSpec   *pspec)
{
  WlrootsSeatKeyboardState *self = WLROOTS_SEAT_KEYBOARD_STATE (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_SEAT_KEYBOARD_STATE:
      self->wrapped_state = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_seat_keyboard_state_class_init (WlrootsSeatKeyboardStateClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_seat_keyboard_state_finalize;
  object_class->get_property = wlroots_seat_keyboard_state_get_property;
  object_class->set_property = wlroots_seat_keyboard_state_set_property;

  properties [PROP_WLROOTS_SEAT_KEYBOARD_STATE] =
    g_param_spec_pointer ("wlroots-seat-keyboard-state",
                          "WlrootsSeatKeyboardState",
                          "WlrootsSeatKeyboardState",

                          (G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS));

  properties[PROP_FOCUSED_SURFACE] =
    g_param_spec_object ("focused-surface",
                         "FocusedSurface",
                         "FocusedSurface",
                         WLROOTS_TYPE_SURFACE,
                         (G_PARAM_READABLE |
                          G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
wlroots_seat_keyboard_state_init (WlrootsSeatKeyboardState *self)
{
}
