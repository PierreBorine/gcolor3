/* Utilities
 *
 * Copyright (C) 2018 Jente Hidskes
 *
 * Author: Jente Hidskes <hjdskes@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <glib.h>
#include <gdk/gdk.h>

G_BEGIN_DECLS

extern gchar *gcolor3_cli_arg_color;
extern gboolean is_gcolor3_cli_arg_color;

double scale_round (double val, double factor);
gchar *hex_value (GdkRGBA *color);

G_END_DECLS

#endif /* __UTILS_H__ */
