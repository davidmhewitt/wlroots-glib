/* wlroots-wayland-list.c
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

#include "wlroots-wayland-list.h"

struct _WlrootsWaylandList
{
  GObject parent_instance;
};

G_DEFINE_TYPE (WlrootsWaylandList, wlroots_wayland_list, G_TYPE_OBJECT)

enum {
  PROP_0,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

WlrootsWaylandList *
wlroots_wayland_list_new (void)
{
  return g_object_new (WLROOTS_TYPE_WAYLAND_LIST, NULL);
}

static void
wlroots_wayland_list_finalize (GObject *object)
{
  WlrootsWaylandList *self = (WlrootsWaylandList *)object;

  G_OBJECT_CLASS (wlroots_wayland_list_parent_class)->finalize (object);
}

static void
wlroots_wayland_list_get_property (GObject    *object,
                                   guint       prop_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
  WlrootsWaylandList *self = WLROOTS_WAYLAND_LIST (object);

  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_wayland_list_set_property (GObject      *object,
                                   guint         prop_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
  WlrootsWaylandList *self = WLROOTS_WAYLAND_LIST (object);

  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_wayland_list_class_init (WlrootsWaylandListClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_wayland_list_finalize;
  object_class->get_property = wlroots_wayland_list_get_property;
  object_class->set_property = wlroots_wayland_list_set_property;
}

static void
wlroots_wayland_list_init (WlrootsWaylandList *self)
{
}
