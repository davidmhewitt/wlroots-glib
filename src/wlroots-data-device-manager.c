/* wlroots-data-device-manager.c
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

#include "wlroots-data-device-manager.h"
#include <wlr/types/wlr_data_device.h>

struct _WlrootsDataDeviceManager
{
  GObject parent_instance;

  struct wlr_data_device_manager *wrapped_ddm;
};

G_DEFINE_TYPE (WlrootsDataDeviceManager, wlroots_data_device_manager, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WLROOTS_DATA_DEVICE_MANAGER,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

WlrootsDataDeviceManager *
wlroots_data_device_manager_new (void)
{
  return g_object_new (WLROOTS_TYPE_DATA_DEVICE_MANAGER, NULL);
}

/**
 * wlroots_data_device_manager_create:
 * @display: (transfer none): The #WlrootsWaylandDisplay to setup the device manager with
 *
 * Returns: (transfer full): A new #WlrootsDataDeviceManager instance
 *
 * Since: 0.1
 */
WlrootsDataDeviceManager *
wlroots_data_device_manager_create (WlrootsWaylandDisplay *display)
{
  struct wl_display *way_display = wlroots_wayland_display_get_wl_display (display);

  struct wlr_data_device_manager *ddm = wlr_data_device_manager_create (way_display);
  return g_object_new (WLROOTS_TYPE_DATA_DEVICE_MANAGER, "wlroots-data-device-manager", ddm, NULL);
}

static void
wlroots_data_device_manager_finalize (GObject *object)
{
  WlrootsDataDeviceManager *self = (WlrootsDataDeviceManager *)object;

  G_OBJECT_CLASS (wlroots_data_device_manager_parent_class)->finalize (object);
}

static void
wlroots_data_device_manager_get_property (GObject    *object,
                                          guint       prop_id,
                                          GValue     *value,
                                          GParamSpec *pspec)
{
  WlrootsDataDeviceManager *self = WLROOTS_DATA_DEVICE_MANAGER (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_DATA_DEVICE_MANAGER:
      g_value_set_pointer (value, self->wrapped_ddm);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_data_device_manager_set_property (GObject      *object,
                                          guint         prop_id,
                                          const GValue *value,
                                          GParamSpec   *pspec)
{
  WlrootsDataDeviceManager *self = WLROOTS_DATA_DEVICE_MANAGER (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_DATA_DEVICE_MANAGER:
      self->wrapped_ddm = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_data_device_manager_class_init (WlrootsDataDeviceManagerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_data_device_manager_finalize;
  object_class->get_property = wlroots_data_device_manager_get_property;
  object_class->set_property = wlroots_data_device_manager_set_property;

  properties [PROP_WLROOTS_DATA_DEVICE_MANAGER] =
    g_param_spec_pointer ("wlroots-data-device-manager",
                          "WlrootsDataDeviceManager",
                          "WlrootsDataDeviceManager",
                          (G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS));
  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
wlroots_data_device_manager_init (WlrootsDataDeviceManager *self)
{
}
