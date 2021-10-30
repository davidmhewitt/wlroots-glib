/* wlroots-backend.c
 *
 * Copyright 2019-2021 David Hewitt <davidmhewitt@gmail.com>
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

#include "wlroots-backend.h"
#include "wlroots-input-device.h"
#include "wlroots-output.h"
#include "wlroots-wayland-display.h"
#include "wlroots-renderer.h"
#include <wlr/render/wlr_renderer.h>

struct _WlrootsBackend
{
  GObject parent_instance;

  struct wl_display *wayland_display;
  struct wlr_backend *wrapped_backend;

  struct wl_listener new_input;
  struct wl_listener new_output;
};

G_DEFINE_TYPE (WlrootsBackend, wlroots_backend, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WAYLAND_DISPLAY,
  N_PROPS
};

enum {
  NEW_INPUT,
  NEW_OUTPUT,
  N_SIGNALS
};

static GParamSpec *properties [N_PROPS];
static guint signals[N_SIGNALS];

static struct wl_display *
wlroots_backend_get_wayland_display (WlrootsBackend *self)
{
  return self->wayland_display;
}

static void
wlroots_backend_set_wayland_display (WlrootsBackend *self, struct wl_display *display) {
  self->wayland_display = display;
}


WlrootsBackend *
wlroots_backend_new (void)
{
  return g_object_new (WLROOTS_TYPE_BACKEND, NULL);
}

/**
 * wlroots_backend_autocreate: (constructor):
 * @display: (transfer none): #WlrootsWaylandDisplay to autocreate backend from
 *
 * Returns: (transfer full): Newly created #WlrootsBackend
 *
 * Since: 0.1
 */
WlrootsBackend *
wlroots_backend_autocreate (WlrootsWaylandDisplay *display)
{
  struct wl_display *way_display = wlroots_wayland_display_get_wl_display (display);
  return g_object_new (WLROOTS_TYPE_BACKEND, "wayland-display", way_display, NULL);
}

/**
 * wlroots_backend_get_renderer:
 *
 * Returns: (transfer full): A new #WlrootsRenderer instance
 *
 * Since: 0.1
 */
WlrootsRenderer *
wlroots_backend_get_renderer (WlrootsBackend *self)
{
  struct wlr_renderer *renderer = wlr_backend_get_renderer (self->wrapped_backend);
  return wlroots_renderer_wrap (renderer);
}

gboolean
wlroots_backend_start (WlrootsBackend *self)
{
  return wlr_backend_start (self->wrapped_backend);
}

static void
wlroots_backend_finalize (GObject *object)
{
  WlrootsBackend *self = (WlrootsBackend *)object;

  if (self->wrapped_backend)
    {
      wlr_backend_destroy (self->wrapped_backend);
    }

  G_OBJECT_CLASS (wlroots_backend_parent_class)->finalize (object);
}

static void
wlroots_backend_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  WlrootsBackend *self = WLROOTS_BACKEND (object);

  switch (prop_id)
    {
    case PROP_WAYLAND_DISPLAY:
      g_value_set_pointer (value, wlroots_backend_get_wayland_display (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_backend_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  WlrootsBackend *self = WLROOTS_BACKEND (object);

  switch (prop_id)
    {
    case PROP_WAYLAND_DISPLAY:
      wlroots_backend_set_wayland_display (self, g_value_get_pointer (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
backend_new_output (struct wl_listener *listener, void *data)
{
  WlrootsBackend *self = wl_container_of (listener, self, new_output);
  WlrootsOutput *output = wlroots_output_wrap (data);

  g_signal_emit (self, signals[NEW_OUTPUT], 0, output);
}

static void
backend_new_input (struct wl_listener *listener, void *data)
{
  WlrootsBackend *self = wl_container_of (listener, self, new_input);
  WlrootsInputDevice *device = wlroots_input_device_wrap (data);

  g_signal_emit (self, signals[NEW_INPUT], 0, device);
}

static void
wlroots_backend_constructed (GObject *obj)
{
  WlrootsBackend *self = WLROOTS_BACKEND (obj);

  self->wrapped_backend = wlr_backend_autocreate (self->wayland_display);
  self->new_input.notify = backend_new_input;
  self->new_output.notify = backend_new_output;

  wl_signal_add (&self->wrapped_backend->events.new_input, &self->new_input);
  wl_signal_add (&self->wrapped_backend->events.new_output, &self->new_output);

  G_OBJECT_CLASS(wlroots_backend_parent_class)->constructed (obj);
}

static void
wlroots_backend_class_init (WlrootsBackendClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_backend_finalize;
  object_class->get_property = wlroots_backend_get_property;
  object_class->set_property = wlroots_backend_set_property;
  object_class->constructed = wlroots_backend_constructed;

  properties [PROP_WAYLAND_DISPLAY] =
    g_param_spec_pointer ("wayland-display",
                          "WaylandDisplay",
                          "WaylandDisplay",
                          G_PARAM_READWRITE |
                          G_PARAM_CONSTRUCT_ONLY |
                          G_PARAM_STATIC_STRINGS);
  g_object_class_install_properties (object_class, N_PROPS, properties);

  /**
   * WlrootsBackend::new-input:
   * @backend: The #WlrootsBackend that received the signal
   * @input: (transfer none): The new #WlrootsInputDevice
   *
   * Since: 0.1
   */
  signals[NEW_INPUT] =
    g_signal_new ("new-input",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  1,
                  WLROOTS_TYPE_INPUT_DEVICE);

  /**
   * WlrootsBackend::new-output:
   * @backend: The #WlrootsBackend that received the signal
   * @output: (transfer none): The new #WlrootsOutput
   *
   * Since: 0.1
   */
  signals[NEW_OUTPUT] =
    g_signal_new ("new-output",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  1,
                  WLROOTS_TYPE_OUTPUT);
}

static void
wlroots_backend_init (WlrootsBackend *self)
{

}
