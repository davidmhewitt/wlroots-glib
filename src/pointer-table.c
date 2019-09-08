/* pointer-table.c
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

#include "pointer-table.h"

GHashTable *wlroots_pointers = NULL;

GObject *
lookup_pointer (void *pointer)
{
  GObject *existing = NULL;

  if (!wlroots_pointers)
  {
    wlroots_pointers = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, g_object_unref);
  }

  existing = g_hash_table_lookup (wlroots_pointers, pointer);

  if (existing)
  {
    return existing;
  }

  return NULL;
}
