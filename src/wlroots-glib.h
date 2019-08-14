/* wlroots-glib.h
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

#include "wlroots-backend.h"
#include "wlroots-box.h"
#include "wlroots-compositor.h"
#include "wlroots-cursor.h"
#include "wlroots-data-device-manager.h"
#include "wlroots-events.h"
#include "wlroots-input-device.h"
#include "wlroots-output.h"
#include "wlroots-output-layout.h"
#include "wlroots-renderer.h"
#include "wlroots-seat.h"
#include "wlroots-surface.h"
#include "wlroots-texture.h"
#include "wlroots-wayland-display.h"
#include "wlroots-xcursor-manager.h"
#include "wlroots-xdg-shell.h"
#include "wlroots-xdg-surface.h"
#include "wlroots-xdg-toplevel.h"
