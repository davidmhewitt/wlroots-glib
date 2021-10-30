/* wlroots-cursor.c
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

#include "wlroots-cursor.h"
#include "wlroots-events.h"
#include <wlr/types/wlr_cursor.h>

struct _WlrootsCursor
{
  GObject parent_instance;

  struct wlr_cursor *wrapped_cursor;

  struct wl_listener motion;
  struct wl_listener motion_absolute;
  struct wl_listener button;
  struct wl_listener axis;
  struct wl_listener frame;
};

G_DEFINE_TYPE (WlrootsCursor, wlroots_cursor, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_X,
  PROP_Y,
  N_PROPS
};

enum {
  MOTION,
  MOTION_ABSOLUTE,
  BUTTON,
  AXIS,
  FRAME,
  N_SIGNALS
};

static GParamSpec *properties [N_PROPS];
static guint signals [N_SIGNALS];

WlrootsCursor *
wlroots_cursor_new (void)
{
  return g_object_new (WLROOTS_TYPE_CURSOR, NULL);
}

void
wlroots_cursor_attach_output_layout (WlrootsCursor *self, WlrootsOutputLayout *output_layout)
{
  struct wlr_output_layout *wlr_layout = wlroots_output_layout_get_wlr_output_layout (output_layout);
  wlr_cursor_attach_output_layout (self->wrapped_cursor, wlr_layout);
}

void
wlroots_cursor_attach_input_device (WlrootsCursor *self, WlrootsInputDevice *input_dev)
{
  struct wlr_input_device *dev = wlroots_input_device_get_wlr_device (input_dev);
  wlr_cursor_attach_input_device (self->wrapped_cursor, dev);
}

void
wlroots_cursor_warp_absolute (WlrootsCursor *self, WlrootsInputDevice *device, gdouble x, gdouble y)
{
  struct wlr_input_device *wlr_dev = wlroots_input_device_get_wlr_device (device);
  wlr_cursor_warp_absolute (self->wrapped_cursor, wlr_dev, x, y);
}

void
wlroots_cursor_move (WlrootsCursor *self, WlrootsInputDevice *device, gdouble delta_x, gdouble delta_y)
{
  struct wlr_input_device *wlr_dev = wlroots_input_device_get_wlr_device (device);
  wlr_cursor_move (self->wrapped_cursor, wlr_dev, delta_x, delta_y);
}

void
wlroots_cursor_set_surface (WlrootsCursor *self, WlrootsSurface *surface, gint32 hotspot_x, gint32 hotspot_y)
{
  wlr_cursor_set_surface (self->wrapped_cursor, wlroots_surface_get_wlr_surface (surface), hotspot_x, hotspot_y);
}

struct wlr_cursor *
wlroots_cursor_get_wlr_cursor (WlrootsCursor *self)
{
  return self->wrapped_cursor;
}

static void
wlroots_cursor_finalize (GObject *object)
{
  WlrootsCursor *self = (WlrootsCursor *)object;

  G_OBJECT_CLASS (wlroots_cursor_parent_class)->finalize (object);
}

static void
wlroots_cursor_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  WlrootsCursor *self = WLROOTS_CURSOR (object);

  switch (prop_id)
    {
    case PROP_X:
      g_value_set_double (value, self->wrapped_cursor->x);
      break;
    case PROP_Y:
      g_value_set_double (value, self->wrapped_cursor->y);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_cursor_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  WlrootsCursor *self = WLROOTS_CURSOR (object);

  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_cursor_class_init (WlrootsCursorClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_cursor_finalize;
  object_class->get_property = wlroots_cursor_get_property;
  object_class->set_property = wlroots_cursor_set_property;

  properties [PROP_X] =
    g_param_spec_double ("x",
                         "X",
                         "X",
                         -G_MAXDOUBLE,
                         G_MAXDOUBLE,
                         0,

                         (G_PARAM_READABLE |
                          G_PARAM_STATIC_STRINGS));

  properties [PROP_Y] =
    g_param_spec_double ("y",
                         "Y",
                         "Y",
                         -G_MAXDOUBLE,
                         G_MAXDOUBLE,
                         0,
                         (G_PARAM_READABLE |
                          G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);

  signals [MOTION] =
    g_signal_new ("motion",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  1,
                  WLROOTS_TYPE_EVENT_POINTER_MOTION);

  signals [MOTION_ABSOLUTE] =
    g_signal_new ("motion-absolute",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  1,
                  WLROOTS_TYPE_EVENT_POINTER_MOTION_ABSOLUTE);

  signals [BUTTON] =
    g_signal_new ("button",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  1,
                  WLROOTS_TYPE_EVENT_POINTER_BUTTON);

  signals [AXIS] =
    g_signal_new ("axis",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  1,
                  WLROOTS_TYPE_EVENT_POINTER_AXIS);

  signals [FRAME] =
    g_signal_new ("frame",
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
cursor_motion (struct wl_listener *listener, void *data)
{
  WlrootsCursor *self = wl_container_of (listener, self, motion);
  WlrootsEventPointerMotion *ev = wlroots_event_pointer_motion_new (data);

  g_signal_emit (self, signals[MOTION], 0, ev);
}

static void
cursor_motion_absolute (struct wl_listener *listener, void *data)
{
  WlrootsCursor *self = wl_container_of (listener, self, motion_absolute);
  WlrootsEventPointerMotionAbsolute *ev = wlroots_event_pointer_motion_absolute_new (data);

  g_signal_emit (self, signals[MOTION_ABSOLUTE], 0, ev);
}

static void
cursor_button (struct wl_listener *listener, void *data)
{
  WlrootsCursor *self = wl_container_of (listener, self, button);
  WlrootsEventPointerButton *ev = wlroots_event_pointer_button_new (data);

  g_signal_emit (self, signals[BUTTON], 0, ev);
}

static void
cursor_axis (struct wl_listener *listener, void *data)
{
  WlrootsCursor *self = wl_container_of (listener, self, axis);
  WlrootsEventPointerAxis *ev = wlroots_event_pointer_axis_new (data);

  g_signal_emit (self, signals[AXIS], 0, ev);
}

static void
cursor_frame (struct wl_listener *listener, void *data)
{
  WlrootsCursor *self = wl_container_of (listener, self, frame);

  g_signal_emit (self, signals[FRAME], 0);
}

static void
wlroots_cursor_init (WlrootsCursor *self)
{
  self->wrapped_cursor = wlr_cursor_create ();

  self->motion.notify = cursor_motion;
  self->motion_absolute.notify = cursor_motion_absolute;
  self->button.notify = cursor_button;
  self->axis.notify = cursor_axis;
  self->frame.notify = cursor_frame;

  wl_signal_add (&self->wrapped_cursor->events.motion, &self->motion);
  wl_signal_add (&self->wrapped_cursor->events.motion_absolute, &self->motion_absolute);
  wl_signal_add (&self->wrapped_cursor->events.button, &self->button);
  wl_signal_add (&self->wrapped_cursor->events.axis, &self->axis);
  wl_signal_add (&self->wrapped_cursor->events.frame, &self->frame);
}
