/* wlroots-input.h
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
#include <wlr/types/wlr_input_device.h>

G_BEGIN_DECLS

#define WLROOTS_TYPE_INPUT_DEVICE (wlroots_input_device_get_type())

G_DECLARE_FINAL_TYPE (WlrootsInputDevice, wlroots_input_device, WLROOTS, INPUT_DEVICE, GObject)

WlrootsInputDevice *wlroots_input_device_wrap (struct wlr_input_device *device);
struct wlr_input_device *wlroots_input_device_get_wlr_device (WlrootsInputDevice *self);

typedef enum {
  WLROOTS_INPUT_DEVICE_TYPE_UNKNOWN,
  WLROOTS_INPUT_DEVICE_TYPE_KEYBOARD,
  WLROOTS_INPUT_DEVICE_TYPE_POINTER,
	WLROOTS_INPUT_DEVICE_TYPE_TOUCH,
	WLROOTS_INPUT_DEVICE_TYPE_TABLET_TOOL,
	WLROOTS_INPUT_DEVICE_TYPE_TABLET_PAD,
} WlrootsInputDeviceType;

G_END_DECLS
