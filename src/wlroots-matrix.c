/* wlroots-matrix.c
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

#include "wlroots-matrix.h"
#include <wlr/types/wlr_matrix.h>

/**
 * wlroots_matrix_project_box:
 * @mat: (array fixed-size=9)(out): The returned matrix
 * @projection: (array fixed-size=9): The projection
 *
 * Since: 0.1
 */
void wlroots_matrix_project_box (gfloat mat[],
                                 WlrootsBox *box,
                                 WlrootsWaylandOutputTransform transform,
                                 gfloat rotation,
                                 const gfloat projection[])
{
  struct wlr_box roots_box = {
		.x = box->x,
		.y = box->y,
		.width = box->width,
		.height = box->height,
	};

  wlr_matrix_project_box (mat, &roots_box, transform, rotation, projection);
}


