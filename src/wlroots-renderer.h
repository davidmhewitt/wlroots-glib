/* wlroots-renderer.h
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
#include <wlr/render/wlr_renderer.h>
#include "wlroots-wayland-display.h"

G_BEGIN_DECLS

#define WLROOTS_TYPE_RENDERER (wlroots_renderer_get_type())

G_DECLARE_FINAL_TYPE (WlrootsRenderer, wlroots_renderer, WLROOTS, RENDERER, GObject)

WlrootsRenderer *wlroots_renderer_wrap (struct wlr_renderer *renderer);
void wlroots_renderer_init_wl_display (WlrootsRenderer *self, WlrootsWaylandDisplay *display);
struct wlr_renderer *wlroots_renderer_get_wlr_renderer (WlrootsRenderer *self);
void wlroots_renderer_begin (WlrootsRenderer *self, int width, int height);
void wlroots_renderer_end (WlrootsRenderer *self);
void wlroots_renderer_clear (WlrootsRenderer *self, const float color[]);

G_END_DECLS
