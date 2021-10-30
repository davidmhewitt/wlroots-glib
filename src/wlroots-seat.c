/* wlroots-seat.c
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

#include <wlr/types/wlr_seat.h>
#include "wlroots-seat.h"
#include "wlroots-seat-keyboard-state.h"

struct _WlrootsSeat
{
  GObject parent_instance;

  gchar *name;
  struct wl_display *wayland_display;

  struct wlr_seat *wrapped_seat;

  WlrootsSeatKeyboardState *keyboard_state;

  struct wl_listener request_cursor;
};

G_DEFINE_TYPE (WlrootsSeat, wlroots_seat, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WAYLAND_DISPLAY,
  PROP_NAME,
  PROP_KEYBOARD_STATE,
  N_PROPS
};

enum {
  REQUEST_CURSOR,
  N_SIGNALS
};

static GParamSpec *properties [N_PROPS];
static guint signals [N_SIGNALS];

WlrootsSeat *
wlroots_seat_new (WlrootsWaylandDisplay *display, gchar *name)
{
  struct wl_display *way_display = wlroots_wayland_display_get_wl_display (display);
  return g_object_new (WLROOTS_TYPE_SEAT, "name", name, "wayland-display", way_display, NULL);
}

void
wlroots_seat_set_capabilities (WlrootsSeat *self, WlrootsWaylandSeatCapability caps)
{
  wlr_seat_set_capabilities (self->wrapped_seat, caps);
}

void
wlroots_seat_set_keyboard (WlrootsSeat *self, WlrootsInputDevice *device)
{
  wlr_seat_set_keyboard (self->wrapped_seat, wlroots_input_device_get_wlr_device (device));
}

void
wlroots_seat_pointer_notify_axis (WlrootsSeat                 *self,
                                  guint32                      msec,
                                  WlrootsEventAxisOrientation  orientation,
                                  gdouble                      delta,
                                  gint32                       delta_discrete,
                                  WlrootsEventAxisSource       source)
{
  wlr_seat_pointer_notify_axis (self->wrapped_seat,
                                msec,
                                orientation,
                                delta,
                                delta_discrete,
                                source);
}

void
wlroots_seat_pointer_notify_button (WlrootsSeat        *self,
                                    guint32             msec,
                                    guint32             button,
                                    WlrootsButtonState  state)
{
  wlr_seat_pointer_notify_button (self->wrapped_seat,
                                  msec,
                                  button,
                                  state);
}

void
wlroots_seat_pointer_notify_enter (WlrootsSeat    *self,
                                   WlrootsSurface *surface,
                                   gdouble         sx,
                                   gdouble         sy)
{
  struct wlr_surface *_surface;
  _surface = wlroots_surface_get_wlr_surface (surface);
  wlr_seat_pointer_notify_enter (self->wrapped_seat, _surface, sx, sy);
}

void
wlroots_seat_pointer_clear_focus (WlrootsSeat *self)
{
  wlr_seat_pointer_clear_focus (self->wrapped_seat);
}

void
wlroots_seat_pointer_notify_motion (WlrootsSeat *self,
                                    guint32      msec,
                                    gdouble      x,
                                    gdouble      y)
{
  wlr_seat_pointer_notify_motion (self->wrapped_seat, msec, x, y);
}

static void
wlroots_seat_finalize (GObject *object)
{
  WlrootsSeat *self = (WlrootsSeat *)object;

  G_OBJECT_CLASS (wlroots_seat_parent_class)->finalize (object);
}

static void
wlroots_seat_get_property (GObject    *object,
                           guint       prop_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
  WlrootsSeat *self = WLROOTS_SEAT (object);

  switch (prop_id)
    {
    case PROP_WAYLAND_DISPLAY:
      g_value_set_pointer (value, self->wayland_display);
      break;
    case PROP_NAME:
      g_value_set_string (value, self->name);
      break;
    case PROP_KEYBOARD_STATE:
      if (self->keyboard_state == NULL) {
        self->keyboard_state = wlroots_seat_keyboard_state_wrap (&self->wrapped_seat->keyboard_state);
      }

      g_value_set_object (value, self->keyboard_state);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_seat_set_property (GObject      *object,
                           guint         prop_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
  WlrootsSeat *self = WLROOTS_SEAT (object);

  switch (prop_id)
    {
    case PROP_WAYLAND_DISPLAY:
      self->wayland_display = g_value_get_pointer (value);
      break;
    case PROP_NAME:
      if(self->name) {
        g_free (self->name);
      }
      self->name = g_value_dup_string (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
seat_request_cursor (struct wl_listener *listener, void *data)
{
  WlrootsSeat *self = wl_container_of (listener, self, request_cursor);
  WlrootsEventRequestCursor *event = wlroots_event_request_cursor_new ((struct wlr_seat_pointer_request_set_cursor_event *)data);
  g_signal_emit (self, signals[REQUEST_CURSOR], 0, event);
}

static void
wlroots_seat_constructed (GObject *obj)
{
  WlrootsSeat *self = WLROOTS_SEAT (obj);

  self->wrapped_seat = wlr_seat_create (self->wayland_display, self->name);

  self->request_cursor.notify = seat_request_cursor;
  wl_signal_add (&self->wrapped_seat->events.request_set_cursor, &self->request_cursor);

  G_OBJECT_CLASS (wlroots_seat_parent_class)->constructed (obj);
}

static void
wlroots_seat_class_init (WlrootsSeatClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_seat_finalize;
  object_class->get_property = wlroots_seat_get_property;
  object_class->set_property = wlroots_seat_set_property;
  object_class->constructed = wlroots_seat_constructed;

  properties [PROP_NAME] =
    g_param_spec_string ("name",
                         "Name",
                         "Name",
                         NULL,

                         (G_PARAM_READWRITE |
                          G_PARAM_CONSTRUCT_ONLY |
                          G_PARAM_STATIC_STRINGS));

  properties [PROP_WAYLAND_DISPLAY] =
    g_param_spec_pointer ("wayland-display",
                          "WaylandDisplay",
                          "WaylandDisplay",

                          (G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS));

  properties [PROP_KEYBOARD_STATE] =
    g_param_spec_object ("keyboard-state",
                         "KeyboardState",
                         "KeyboardState",
                         WLROOTS_TYPE_SEAT_KEYBOARD_STATE,
                         (G_PARAM_READABLE |
                          G_PARAM_STATIC_STRINGS));
  g_object_class_install_properties (object_class, N_PROPS, properties);

  signals [REQUEST_CURSOR] =
    g_signal_new ("request-cursor",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  1,
                  WLROOTS_TYPE_EVENT_REQUEST_CURSOR);
}

static void
wlroots_seat_init (WlrootsSeat *self)
{
}
