/* wlroots-output-layout.c
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

#include "wlroots-output-layout.h"
#include <wlr/types/wlr_output_layout.h>

struct _WlrootsOutputLayout
{
  GObject parent_instance;

  struct wlr_output_layout *wrapped_output_layout;
};

G_DEFINE_TYPE (WlrootsOutputLayout, wlroots_output_layout, G_TYPE_OBJECT)

enum {
  PROP_0,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

WlrootsOutputLayout *
wlroots_output_layout_new (void)
{
  return g_object_new (WLROOTS_TYPE_OUTPUT_LAYOUT, NULL);
}

void
wlroots_output_layout_add_auto (WlrootsOutputLayout *self, WlrootsOutput *output) {
  struct wlr_output *roots_output = wlroots_output_get_wlr_output (output);
  wlr_output_layout_add_auto (self->wrapped_output_layout, roots_output);
}

struct wlr_output_layout *
wlroots_output_layout_get_wlr_output_layout (WlrootsOutputLayout *self)
{
  return self->wrapped_output_layout;
}

/**
 * wlroots_output_layout_output_coords:
 * @output: (transfer none): The #WlrootsOutput to get the position of
 * @ox: (out): Pointer to double where to return x value
 * @oy: (out): Pointer to double where to return y value
 *
 * Since: 0.1
 */
void
wlroots_output_layout_output_coords (WlrootsOutputLayout *self, WlrootsOutput *output, gdouble *ox, gdouble *oy)
{
  struct wlr_output *wlroots_output = wlroots_output_get_wlr_output (output);
  wlr_output_layout_output_coords (self->wrapped_output_layout, output, ox, oy);
}

static void
wlroots_output_layout_finalize (GObject *object)
{
  WlrootsOutputLayout *self = (WlrootsOutputLayout *)object;

  wlr_output_layout_destroy (self->wrapped_output_layout);

  G_OBJECT_CLASS (wlroots_output_layout_parent_class)->finalize (object);
}

static void
wlroots_output_layout_get_property (GObject    *object,
                                    guint       prop_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
  WlrootsOutputLayout *self = WLROOTS_OUTPUT_LAYOUT (object);

  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_output_layout_set_property (GObject      *object,
                                    guint         prop_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
  WlrootsOutputLayout *self = WLROOTS_OUTPUT_LAYOUT (object);

  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
wlroots_output_layout_class_init (WlrootsOutputLayoutClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = wlroots_output_layout_finalize;
  object_class->get_property = wlroots_output_layout_get_property;
  object_class->set_property = wlroots_output_layout_set_property;
}

static void
wlroots_output_layout_init (WlrootsOutputLayout *self)
{
  self->wrapped_output_layout = wlr_output_layout_create ();
}
