/* wlroots-seat-keyboard-state.h
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
#include <wlr/types/wlr_seat.h>
#include "wlroots-surface.h"

G_BEGIN_DECLS

#define WLROOTS_TYPE_SEAT_KEYBOARD_STATE (wlroots_seat_keyboard_state_get_type())

G_DECLARE_FINAL_TYPE (WlrootsSeatKeyboardState, wlroots_seat_keyboard_state, WLROOTS, SEAT_KEYBOARD_STATE, GObject)

WlrootsSeatKeyboardState *wlroots_seat_keyboard_state_wrap (struct wlr_seat_keyboard_state *state);
WlrootsSurface *wlroots_seat_keyboard_state_get_focused_surface (WlrootsSeatKeyboardState *self);

G_END_DECLS
