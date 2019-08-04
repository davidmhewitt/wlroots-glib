/* wlroots-input.c
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

#include "wlroots-input-device.h"
#include "wlroots-enum-types.h"

struct _WlrootsInputDevice
{
  GObject parent_instance;

  struct wlr_input_device *wrapped_device;
};

G_DEFINE_TYPE (WlrootsInputDevice, wlroots_input_device, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WLROOTS_INPUT_DEVICE,
  PROP_DEVICE_TYPE,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

WlrootsInputDevice *
wlroots_input_device_new (void)
{
  return g_object_new (WLROOTS_TYPE_INPUT_DEVICE, NULL);
}

/**
 * wlroots_input_device_wrap:
 * @device: (transfer none): The #wlr_input_device to wrap
 *
 * Returns: (transfer full): A new #WlrootsInputDevice instance
 *
 * Since: 0.1
 */
WlrootsInputDevice *
wlroots_input_device_wrap (struct wlr_input_device *device)
{
  return g_object_new (WLROOTS_TYPE_INPUT_DEVICE, "wlroots-input-device", device, NULL);
}

struct wlr_input_device *
wlroots_input_device_get_wlr_device (WlrootsInputDevice *self)
{
  return self->wrapped_device;
}

static void
wlroots_input_device_finalize (GObject *object)
{
  WlrootsInputDevice *self = (WlrootsInputDevice *)object;

  G_OBJECT_CLASS (wlroots_input_device_parent_class)->finalize (object);
}

static gint
wlr_device_type_to_wlroots_device_type (gint type)
{
  gint output = WLROOTS_INPUT_DEVICE_TYPE_UNKNOWN;
  switch(type)
    {
    case WLR_INPUT_DEVICE_KEYBOARD:
      output = WLROOTS_INPUT_DEVICE_TYPE_KEYBOARD;
      break;
    case WLR_INPUT_DEVICE_POINTER:
      output = WLROOTS_INPUT_DEVICE_TYPE_POINTER;
      break;
    case WLR_INPUT_DEVICE_TOUCH:
      output = WLROOTS_INPUT_DEVICE_TYPE_TOUCH;
      break;
    case WLR_INPUT_DEVICE_TABLET_PAD:
      output = WLROOTS_INPUT_DEVICE_TYPE_TABLET_PAD;
      break;
    case WLR_INPUT_DEVICE_TABLET_TOOL:
      output = WLROOTS_INPUT_DEVICE_TYPE_TABLET_TOOL;
      break;
    default:
      break;
    }

  return output;
}

static void
wlroots_input_device_get_property (GObject    *object,
                                   guint       prop_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
  WlrootsInputDevice *self = WLROOTS_INPUT_DEVICE (object);
  gint dev_type;

  switch (prop_id)
    {
    case PROP_WLROOTS_INPUT_DEVICE:
      g_value_set_pointer (value, self->wrapped_device);
      break;
    case PROP_DEVICE_TYPE:
      dev_type = wlr_device_type_to_wlroots_device_type (self->wrapped_device->type);
      g_value_set_enum (value, dev_type);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_input_device_set_property (GObject      *object,
                                   guint         prop_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
  WlrootsInputDevice *self = WLROOTS_INPUT_DEVICE (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_INPUT_DEVICE:
      self->wrapped_device = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_input_device_constructed (GObject *obj)
{
  WlrootsInputDevice *self = WLROOTS_INPUT_DEVICE (obj);

  G_OBJECT_CLASS(wlroots_input_device_parent_class)->constructed (obj);
}

static void
wlroots_input_device_class_init (WlrootsInputDeviceClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_input_device_finalize;
  object_class->get_property = wlroots_input_device_get_property;
  object_class->set_property = wlroots_input_device_set_property;
  object_class->constructed = wlroots_input_device_constructed;

  properties [PROP_WLROOTS_INPUT_DEVICE] =
    g_param_spec_pointer ("wlroots-input-device",
                          "WlrootsInputDevice",
                          "WlrootsInputDevice",

                          (G_PARAM_READWRITE |
                           G_PARAM_CONSTRUCT_ONLY |
                           G_PARAM_STATIC_STRINGS));

  /**
   * WlrootsInputDevice:device-type: (type WlrootsInputDeviceType)
   *
   * The device type.
   */
  properties [PROP_DEVICE_TYPE] =
    g_param_spec_enum ("device-type",
                       "DeviceType",
                       "DeviceType",
                       WLROOTS_TYPE_INPUT_DEVICE_TYPE,
                       WLROOTS_INPUT_DEVICE_TYPE_UNKNOWN,
                       (G_PARAM_READABLE |
                        G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
wlroots_input_device_init (WlrootsInputDevice *self)
{
}
