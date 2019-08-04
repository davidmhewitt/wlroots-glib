/* wlroots-events.c
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

#include "wlroots-events.h"

/**
 * wlroots_event_pointer_motion_absolute_ref:
 * @event: a #WlrootsEventPointerMotionAbsolute.
 *
 * Increments the reference count of the event
 *
 * Returns: the event.
 **/
WlrootsEventPointerMotionAbsolute *
wlroots_event_pointer_motion_absolute_ref (WlrootsEventPointerMotionAbsolute *event)
{
  if (event)
  {
    event->ref_count++;
  }

  return event;
}

/**
 * wlroots_event_pointer_motion_absolute_unref:
 * @event: a #WlrootsEventPointerMotionAbsolute.
 *
 * Decrements the reference count of the event. If it falls to 0
 * it is freed
 *
 * Returns: the event.
 **/
void
wlroots_event_pointer_motion_absolute_unref (WlrootsEventPointerMotionAbsolute *event)
{
  if (event && --event->ref_count == 0)
  {
    g_object_unref (event->device);
    g_free (event);
  }
}

GType
wlroots_event_pointer_motion_absolute_get_type (void)
{
  static GType event_type = 0;

  if (event_type == 0)
    event_type = g_boxed_type_register_static
      ("WlrootsEventPointerMotionAbsolute",
       (GBoxedCopyFunc) wlroots_event_pointer_motion_absolute_ref,
       (GBoxedFreeFunc) wlroots_event_pointer_motion_absolute_unref);

  return event_type;
}

WlrootsEventPointerMotionAbsolute *
wlroots_event_pointer_motion_absolute_new (struct wlr_event_pointer_motion_absolute *wlr_event)
{
  WlrootsEventPointerMotionAbsolute *event;
  event = g_new (WlrootsEventPointerMotionAbsolute, 1);
  event->ref_count = 1;
  event->device = wlroots_input_device_wrap (wlr_event->device);
  event->time_msec = wlr_event->time_msec;
  event->x = wlr_event->x;
  event->y = wlr_event->y;

  return event;
}

/**
 * wlroots_event_pointer_axis_ref:
 * @event: a #WlrootsEventPointerAxis.
 *
 * Increments the reference count of the event
 *
 * Returns: the event.
 **/
WlrootsEventPointerAxis *
wlroots_event_pointer_axis_ref (WlrootsEventPointerAxis *event)
{
  if (event)
  {
    event->ref_count++;
  }

  return event;
}

/**
 * wlroots_event_pointer_axis_unref:
 * @event: a #WlrootsEventPointerAxis.
 *
 * Decrements the reference count of the event. If it falls to 0
 * it is freed
 *
 * Returns: the event.
 **/
void
wlroots_event_pointer_axis_unref (WlrootsEventPointerAxis *event)
{
  if (event && --event->ref_count == 0)
  {
    g_object_unref (event->device);
    g_free (event);
  }
}

GType
wlroots_event_pointer_axis_get_type (void)
{
  static GType event_type = 0;

  if (event_type == 0)
    event_type = g_boxed_type_register_static
      ("WlrootsEventPointerAxis",
       (GBoxedCopyFunc) wlroots_event_pointer_axis_ref,
       (GBoxedFreeFunc) wlroots_event_pointer_axis_unref);

  return event_type;
}

WlrootsEventPointerAxis *
wlroots_event_pointer_axis_new (struct wlr_event_pointer_axis *wlr_event)
{
  WlrootsEventPointerAxis *event;
  event = g_new (WlrootsEventPointerAxis, 1);
  event->ref_count = 1;
  event->device = wlroots_input_device_wrap (wlr_event->device);
  event->time_msec = wlr_event->time_msec;
  event->source = wlr_event->source;
  event->orientation = wlr_event->orientation;
  event->delta = wlr_event->delta;
  event->delta_discrete = wlr_event->delta_discrete;

  return event;
}
