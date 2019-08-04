/* wlroots-xcursor-manager.c
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

#include "wlroots-xcursor-manager.h"
#include <wlr/types/wlr_xcursor_manager.h>

struct _WlrootsXCursorManager
{
  GObject parent_instance;

  gchar *name;
  guint32 size;

  struct wlr_xcursor_manager *wrapped_xcursor_manager;
};

G_DEFINE_TYPE (WlrootsXCursorManager, wlroots_xcursor_manager, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_NAME,
  PROP_SIZE,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

/**
 * wlroots_xcursor_manager_new:
 * @name: (nullable): The name of the cursor manager
 * @size: Cursor size
 *
 * Returns: A new #WlrootsXCursorManager instance
 *
 * Since: 0.1
 */
WlrootsXCursorManager *
wlroots_xcursor_manager_new (gchar *name, guint32 size)
{
  if (!name) {
    name = "";
  }
  return g_object_new (WLROOTS_TYPE_XCURSOR_MANAGER, "name", name, "size", size, NULL);
}

void
wlroots_xcursor_manager_load (WlrootsXCursorManager *self, gfloat scale)
{
  wlr_xcursor_manager_load (self->wrapped_xcursor_manager, scale);
}

static void
wlroots_xcursor_manager_finalize (GObject *object)
{
  WlrootsXCursorManager *self = (WlrootsXCursorManager *)object;

  G_OBJECT_CLASS (wlroots_xcursor_manager_parent_class)->finalize (object);
}

static void
wlroots_xcursor_manager_get_property (GObject    *object,
                                      guint       prop_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
  WlrootsXCursorManager *self = WLROOTS_XCURSOR_MANAGER (object);

  switch (prop_id)
    {
    case PROP_NAME:
      g_value_set_string (value, self->name);
      break;
    case PROP_SIZE:
      g_value_set_uint (value, self->size);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_xcursor_manager_set_property (GObject      *object,
                                      guint         prop_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
  WlrootsXCursorManager *self = WLROOTS_XCURSOR_MANAGER (object);

  switch (prop_id)
    {
    case PROP_NAME:
      if (self->name)
        {
          g_free (self->name);
        }
      self->name = g_value_dup_string (value);
      break;
    case PROP_SIZE:
      self->size = g_value_get_uint (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_xcursor_manager_constructed (GObject *obj)
{
  WlrootsXCursorManager *self = WLROOTS_XCURSOR_MANAGER (obj);

  self->wrapped_xcursor_manager = wlr_xcursor_manager_create (self->name, self->size);

  G_OBJECT_CLASS(wlroots_xcursor_manager_parent_class)->constructed (obj);
}

static void
wlroots_xcursor_manager_class_init (WlrootsXCursorManagerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_xcursor_manager_finalize;
  object_class->get_property = wlroots_xcursor_manager_get_property;
  object_class->set_property = wlroots_xcursor_manager_set_property;
  object_class->constructed = wlroots_xcursor_manager_constructed;

  properties [PROP_SIZE] =
    g_param_spec_uint ("size",
                       "Size",
                       "Size",
                       0, -1,
                       24,
                       (G_PARAM_WRITABLE |
                        G_PARAM_CONSTRUCT_ONLY |
                        G_PARAM_STATIC_STRINGS));

  properties [PROP_NAME] =
    g_param_spec_string ("name",
                         "Name",
                         "Name",
                          NULL,
                         (G_PARAM_WRITABLE |
                          G_PARAM_CONSTRUCT_ONLY |
                          G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
wlroots_xcursor_manager_init (WlrootsXCursorManager *self)
{
}
