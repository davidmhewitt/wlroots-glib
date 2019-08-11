/* wlroots-xdg-surface.h
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
#include <wlr/types/wlr_xdg_shell.h>

G_BEGIN_DECLS

#define WLROOTS_TYPE_XDG_SURFACE (wlroots_xdg_surface_get_type())

G_DECLARE_FINAL_TYPE (WlrootsXDGSurface, wlroots_xdg_surface, WLROOTS, XDG_SURFACE, GObject)

WlrootsXDGSurface *wlroots_xdg_surface_wrap (struct wlr_xdg_surface *surface);

G_END_DECLS
