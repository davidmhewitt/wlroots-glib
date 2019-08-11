/* wlroots-box.c
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

#include "wlroots-box.h"

/**
 * wlroots_box_ref:
 * @box: a #WlrootsBox.
 *
 * Increments the reference count of the box
 *
 * Returns: the box.
 **/
WlrootsBox *
wlroots_box_ref (WlrootsBox *box)
{
  if (box)
  {
    box->ref_count++;
  }

  return box;
}

/**
 * wlroots_box_unref:
 * @box: a #WlrootsBox.
 *
 * Decrements the reference count of the box. If it falls to 0
 * it is freed
 *
 * Returns: the box.
 **/
void
wlroots_box_unref (WlrootsBox *box)
{
  if (box && --box->ref_count == 0)
  {
    g_free (box);
  }
}

GType
wlroots_box_get_type (void)
{
  static GType box_type = 0;

  if (box_type == 0)
    box_type = g_boxed_type_register_static
      ("WlrootsBox",
       (GBoxedCopyFunc) wlroots_box_ref,
       (GBoxedFreeFunc) wlroots_box_unref);

  return box_type;
}

WlrootsBox *
wlroots_box_new ()
{
  WlrootsBox *box;
  box = g_new (WlrootsBox, 1);
  box->ref_count = 1;

  return box;
}

WlrootsBox *
wlroots_box_wrap (struct wlr_box *roots_box)
{
  WlrootsBox *box;
  box = g_new (WlrootsBox, 1);
  box->ref_count = 1;
  box->x = roots_box->x;
  box->y = roots_box->y;
  box->width = roots_box->width;
  box->height = roots_box->height;

  return box;
}
