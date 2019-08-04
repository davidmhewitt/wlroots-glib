/* wlroots-events.h
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

#pragma once

#include <glib-object.h>
#include "wlroots-input-device.h"

G_BEGIN_DECLS

typedef struct _WlrootsEventPointerMotionAbsolute WlrootsEventPointerMotionAbsolute;

/**
 * WlrootsEventPointerMotionAbsolute:
 * @ref_count: the reference count of the struct.
 * @device: (transfer full): the device that generated the event.
 * @time_msec: the time of the event.
 * @x: x co-ordinate of event
 * @y: y co-ordinate of event
 *
 * #WlrootsEventPointerMotionAbsolute gives details about a pointer motion event.
 */
struct _WlrootsEventPointerMotionAbsolute
{
  /*< public >*/
  int ref_count;
  WlrootsInputDevice *device;
  guint32 time_msec;
  gdouble x;
  gdouble y;
};

#define WLROOTS_TYPE_EVENT_POINTER_MOTION_ABSOLUTE  (wlroots_event_pointer_motion_absolute_get_type ())
GType              wlroots_event_pointer_motion_absolute_get_type (void) G_GNUC_CONST;

WlrootsEventPointerMotionAbsolute *wlroots_event_pointer_motion_absolute_new (struct wlr_event_pointer_motion_absolute *event);

G_END_DECLS
