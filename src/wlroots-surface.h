/* wlroots-surface.h
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
#include "wlroots-texture.h"

G_BEGIN_DECLS

#define WLROOTS_TYPE_SURFACE (wlroots_surface_get_type())

G_DECLARE_FINAL_TYPE (WlrootsSurface, wlroots_surface, WLROOTS, SURFACE, GObject)

WlrootsSurface *wlroots_surface_wrap (struct wlr_surface *surface);
WlrootsTexture *wlroots_surface_get_texture (WlrootsSurface *self);
G_END_DECLS
