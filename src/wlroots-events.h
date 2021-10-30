/* wlroots-events.h
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

#pragma once

#include <glib-object.h>
#include <wlr/types/wlr_pointer.h>
#include <wlr/types/wlr_seat.h>
#include "wlroots-input-device.h"
#include "wlroots-surface.h"

G_BEGIN_DECLS

typedef struct _WlrootsEventPointerMotionAbsolute WlrootsEventPointerMotionAbsolute;
typedef struct _WlrootsEventPointerAxis WlrootsEventPointerAxis;
typedef struct _WlrootsEventPointerMotion WlrootsEventPointerMotion;
typedef struct _WlrootsEventPointerButton WlrootsEventPointerButton;
typedef struct _WlrootsEventRequestCursor WlrootsEventRequestCursor;

struct _WlrootsEventPointerMotionAbsolute
{
  /*< public >*/
  int ref_count;
  WlrootsInputDevice *device;
  guint32 time_msec;
  gdouble x;
  gdouble y;
};

typedef enum {
  WLROOTS_AXIS_SOURCE_WHEEL,
  WLROOTS_AXIS_SOURCE_FINGER,
  WLROOTS_AXIS_SOURCE_CONTINUOUS,
  WLROOTS_AXIS_SOURCE_WHEEL_TILT,
} WlrootsEventAxisSource;

typedef enum {
  WLROOTS_AXIS_ORIENTATION_VERTICAL,
  WLROOTS_AXIS_ORIENTATION_HORIZONTAL,
} WlrootsEventAxisOrientation;

struct _WlrootsEventPointerAxis
{
  /*< public >*/
  int ref_count;
  WlrootsInputDevice *device;
  guint32 time_msec;
  WlrootsEventAxisSource source;
  WlrootsEventAxisOrientation orientation;
  gdouble delta;
  gint32 delta_discrete;
};

struct _WlrootsEventPointerMotion
{
  /*< public >*/
  int ref_count;
  WlrootsInputDevice *device;
  guint32 time_msec;
  gdouble delta_x;
  gdouble delta_y;
  gdouble unaccel_dx;
  gdouble unaccel_dy;
};

typedef enum {
  WLROOTS_BUTTON_PRESSED,
  WLROOTS_BUTTON_RELEASED,
} WlrootsButtonState;

struct _WlrootsEventPointerButton
{
  /*< public >*/
  int ref_count;
  WlrootsInputDevice *device;
  guint32 time_msec;
  guint32 button;
  WlrootsButtonState state;
};

struct _WlrootsEventRequestCursor
{
  /*< public >*/
  int ref_count;
  WlrootsSurface *surface;
  guint32 serial;
  gint32 hotspot_x, hotspot_y;
};

#define WLROOTS_TYPE_EVENT_POINTER_MOTION_ABSOLUTE  (wlroots_event_pointer_motion_absolute_get_type ())
#define WLROOTS_TYPE_EVENT_POINTER_AXIS  (wlroots_event_pointer_axis_get_type ())
#define WLROOTS_TYPE_EVENT_POINTER_MOTION  (wlroots_event_pointer_motion_get_type ())
#define WLROOTS_TYPE_EVENT_POINTER_BUTTON  (wlroots_event_pointer_button_get_type ())
#define WLROOTS_TYPE_EVENT_REQUEST_CURSOR (wlroots_event_request_cursor_get_type ())
GType              wlroots_event_pointer_motion_absolute_get_type (void) G_GNUC_CONST;
GType              wlroots_event_pointer_axis_get_type (void) G_GNUC_CONST;
GType              wlroots_event_pointer_motion_get_type (void) G_GNUC_CONST;
GType              wlroots_event_pointer_button_get_type (void) G_GNUC_CONST;
GType              wlroots_event_request_cursor_get_type (void) G_GNUC_CONST;

WlrootsEventPointerMotionAbsolute *wlroots_event_pointer_motion_absolute_new (struct wlr_event_pointer_motion_absolute *event);
WlrootsEventPointerAxis *wlroots_event_pointer_axis_new (struct wlr_event_pointer_axis *event);
WlrootsEventPointerMotion *wlroots_event_pointer_motion_new (struct wlr_event_pointer_motion *event);
WlrootsEventPointerButton *wlroots_event_pointer_button_new (struct wlr_event_pointer_button *event);
WlrootsEventRequestCursor *wlroots_event_request_cursor_new (struct wlr_seat_pointer_request_set_cursor_event *event);

G_END_DECLS
