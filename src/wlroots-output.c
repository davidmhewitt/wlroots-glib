/* wlroots-output.c
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

#include "wlroots-output.h"

struct _WlrootsOutput
{
  GObject parent_instance;

  struct wlr_output *wlroots_output;

  struct wl_listener frame;
};

G_DEFINE_TYPE (WlrootsOutput, wlroots_output, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_WLROOTS_OUTPUT,
  PROP_MODES,
  N_PROPS
};

enum {
  FRAME,
  N_SIGNALS
};

static GParamSpec *properties [N_PROPS];
static guint signals[N_SIGNALS];

WlrootsOutput *
wlroots_output_new (void)
{
  return g_object_new (WLROOTS_TYPE_OUTPUT, NULL);
}

/**
 * wlroots_output_wrap: (constructor):
 * @output: (transfer none): The #wlr_output instance to wrap
 *
 * Returns: (transfer full): New #WlrootsOutput instance
 *
 * Since: 0.1
 */
WlrootsOutput *
wlroots_output_wrap (struct wlr_output *output)
{
  return g_object_new (WLROOTS_TYPE_OUTPUT, "wlroots-output", output, NULL);
}

/**
 * wlroots_output_get_modes:
 *
 * Returns: (element-type WlrootsOutputMode) (transfer full): A #GList of available #WlrootsOutputMode
 *
 * Since: 0.1
 */
GList *
wlroots_output_get_modes (WlrootsOutput *self)
{
  GList *modes = NULL;
  struct wl_list *wl_modes = &self->wlroots_output->modes;

  if (wl_list_empty(wl_modes))
    {
      return modes;
    }

  struct wlr_output_mode *e;
  WlrootsOutputMode *new_mode;
  wl_list_for_each (e, wl_modes, link) {
    new_mode = wlroots_output_mode_new (e, e->width, e->height, e->refresh);
    modes = g_list_append (modes, new_mode);
  }

  return modes;
}

void
wlroots_output_set_mode (WlrootsOutput *self, WlrootsOutputMode *mode)
{
  wlr_output_set_mode (self->wlroots_output, mode->wlr_mode);
}

struct wlr_output *
wlroots_output_get_wlr_output (WlrootsOutput *self)
{
  return self->wlroots_output;
}

void
wlroots_output_create_global (WlrootsOutput *self)
{
  wlr_output_create_global (self->wlroots_output);
}

/**
 * wlroots_output_attach_render:
 * @buffer_age: (out) (nullable): Pointer to int where to store buffer age
 *
 * Returns: Whether the render was attached successfully
 *
 * Since: 0.1
 */
gboolean
wlroots_output_attach_render (WlrootsOutput *self, int *buffer_age)
{
  return wlr_output_attach_render (self->wlroots_output, buffer_age);
}

/**
 * wlroots_output_effective_resolution:
 * @width: (out): Pointer to int to store effective width
 * @height: (out): Pointer to int to store effective height
 *
 * Since: 0.1
 */
void
wlroots_output_effective_resolution (WlrootsOutput *self, int *width, int *height)
{
  wlr_output_effective_resolution (self->wlroots_output, width, height);
}

void
wlroots_output_commit (WlrootsOutput *self)
{
  wlr_output_commit (self->wlroots_output);
}

void
wlroots_output_render_software_cursors (WlrootsOutput *self)
{
  wlr_output_render_software_cursors (self->wlroots_output, NULL);
}

static void
wlroots_output_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  WlrootsOutput *self = WLROOTS_OUTPUT (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_OUTPUT:
      g_value_set_pointer (value, self->wlroots_output);
      break;
    case PROP_MODES:
      g_value_set_pointer (value, wlroots_output_get_modes (self));
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_output_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  WlrootsOutput *self = WLROOTS_OUTPUT (object);

  switch (prop_id)
    {
    case PROP_WLROOTS_OUTPUT:
      self->wlroots_output = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
output_frame (struct wl_listener *listener, void *data)
{
  WlrootsOutput *self = wl_container_of (listener, self, frame);

  g_signal_emit (self, signals[FRAME], 0);
}

static void
wlroots_output_constructed (GObject *obj)
{
  WlrootsOutput *self = WLROOTS_OUTPUT (obj);

  self->frame.notify = output_frame;

  wl_signal_add (&self->wlroots_output->events.frame, &self->frame);

  G_OBJECT_CLASS(wlroots_output_parent_class)->constructed (obj);
}

static void
wlroots_output_class_init (WlrootsOutputClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = wlroots_output_get_property;
  object_class->set_property = wlroots_output_set_property;
  object_class->constructed = wlroots_output_constructed;

  properties [PROP_WLROOTS_OUTPUT] =
    g_param_spec_pointer ("wlroots-output",
                          "WlrootsOutput",
                          "WlrootsOutput",
                          G_PARAM_WRITABLE |
                          G_PARAM_CONSTRUCT_ONLY |
                          G_PARAM_STATIC_STRINGS);

  properties [PROP_MODES] =
    g_param_spec_pointer ("modes",
                          "Modes",
                          "Modes",
                          (G_PARAM_READABLE |
                           G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);

  signals[FRAME] =
    g_signal_new ("frame",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  0);
}

/**
 * wlroots_output_mode_ref:
 * @output_mode: a #WlrootsOutputMode.
 *
 * Increments the reference count of the output mode
 *
 * Returns: the output mode.
 **/
WlrootsOutputMode *
wlroots_output_mode_ref (WlrootsOutputMode *output_mode)
{
  if (output_mode)
  {
    output_mode->ref_count++;
  }

  return output_mode;
}

/**
 * wlroots_output_mode_unref:
 * @output_mode: a #WlrootsOutputMode.
 *
 * Decrements the reference count of the output mode. If it falls to 0
 * it is freed
 *
 * Returns: the output mode.
 **/
void
wlroots_output_mode_unref (WlrootsOutputMode *output_mode)
{
  if (output_mode && --output_mode->ref_count == 0)
  {
    g_free (output_mode);
  }
}

GType
wlroots_output_mode_get_type (void)
{
  static GType output_mode_type = 0;

  if (output_mode_type == 0)
    output_mode_type = g_boxed_type_register_static
      ("WlrootsOutputMode",
       (GBoxedCopyFunc) wlroots_output_mode_ref,
       (GBoxedFreeFunc) wlroots_output_mode_unref);

  return output_mode_type;
}

WlrootsOutputMode *
wlroots_output_mode_new (struct wlr_output_mode *wlr_mode, gint32 width, gint32 height, gint32 refresh)
{
  WlrootsOutputMode *mode;
  mode = g_new (WlrootsOutputMode, 1);
  mode->wlr_mode = wlr_mode;
  mode->ref_count = 1;
  mode->width = width;
  mode->height = height;
  mode->refresh = refresh;

  return mode;
}


static void
wlroots_output_init (WlrootsOutput *self)
{
}
