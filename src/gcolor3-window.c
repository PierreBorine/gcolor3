/* Gcolor3Window
 *
 * Copyright (C) 2015-2018 Jente Hidskes
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

#include "config.h"

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <glib/gi18n.h>

#include "gcolor3-color-item.h"
#include "gcolor3-color-selection.h"
#include "gcolor3-window.h"
#include "utils.h"

struct _Gcolor3WindowPrivate {
	GtkWidget *page_stack;
	GtkWidget *picker;
	GtkWidget *list_stack;
	GtkWidget *scroll;
	GtkWidget *listbox;
	GtkWidget *empty_placeholder;

	GdkRGBA current;
};

G_DEFINE_TYPE_WITH_PRIVATE (Gcolor3Window, gcolor3_window, GTK_TYPE_APPLICATION_WINDOW)

static gboolean
gcolor3_window_picker_page_key_handler (GtkWidget   *widget,
					GdkEventKey *event,
					gpointer     user_data)
{
	if (event->type != GDK_KEY_PRESS) {
		return GDK_EVENT_PROPAGATE;
	}

	switch (event->keyval) {
	case GDK_KEY_s:
		if ((event->state & GDK_CONTROL_MASK) != GDK_CONTROL_MASK) {
			return GDK_EVENT_PROPAGATE;
		}
		return GDK_EVENT_STOP;
	default:
		return GDK_EVENT_PROPAGATE;
	}
}

static void
gcolor3_window_action_change_page (UNUSED GSimpleAction *action,
				   UNUSED GVariant      *parameter,
				   gpointer              user_data)
{
	Gcolor3WindowPrivate *priv;
	const gchar *page;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	page = gtk_stack_get_visible_child_name (GTK_STACK (priv->page_stack));
	if (g_strcmp0 (page, "saved-colors") == 0) {
		gtk_stack_set_visible_child_name (GTK_STACK (priv->page_stack), "picker");
	} else {
		gtk_stack_set_visible_child_name (GTK_STACK (priv->page_stack), "saved-colors");
	}
}

static const GActionEntry window_actions[] = {
	{ "change-page", gcolor3_window_action_change_page, NULL, NULL, NULL },
};

static void
gcolor3_window_picker_changed (Gcolor3ColorSelection *picker, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	gcolor3_color_selection_get_current_color (GCOLOR3_COLOR_SELECTION (picker), &priv->current);
}

static void
gcolor3_window_dispose (GObject *object)
{
	G_OBJECT_CLASS (gcolor3_window_parent_class)->dispose (object);
}

static void
gcolor3_window_finalize (GObject *object)
{
	Gcolor3WindowPrivate *priv;
	gchar *hex;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (object));

	hex = hex_value (&priv->current);
	g_printf ("%s\n", hex);
	g_free (hex);

	G_OBJECT_CLASS (gcolor3_window_parent_class)->finalize (object);
}

static void
gcolor3_window_class_init (Gcolor3WindowClass *gcolor3_window_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (gcolor3_window_class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (gcolor3_window_class);

	object_class->dispose = gcolor3_window_dispose;
	object_class->finalize = gcolor3_window_finalize;

	gtk_widget_class_set_template_from_resource (widget_class, "/nl/hjdskes/gcolor3/window.ui");

	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, page_stack);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, list_stack);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, scroll);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, listbox);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, empty_placeholder);

	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_picker_changed);
	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_picker_page_key_handler);
}

static void
gcolor3_window_init (Gcolor3Window *window)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (window);

	gtk_widget_init_template (GTK_WIDGET (window));

	/* Add the custom color selection widget. */
	priv->picker = gcolor3_color_selection_new ();
	gtk_widget_set_valign (priv->picker, GTK_ALIGN_CENTER);
	gtk_widget_set_halign (priv->picker, GTK_ALIGN_CENTER);
	gtk_widget_set_margin_top (priv->picker, 24);
	gtk_widget_set_margin_bottom (priv->picker, 24);
	gtk_widget_set_margin_start (priv->picker, 24);
	gtk_widget_set_margin_end (priv->picker, 24);
	g_signal_connect (priv->picker, "color-changed",
			  G_CALLBACK (gcolor3_window_picker_changed), window);
	g_signal_connect (priv->picker, "key-press-event",
			  G_CALLBACK (gcolor3_window_picker_page_key_handler), window);
	gtk_stack_add_titled (GTK_STACK (priv->page_stack), priv->picker, "picker", _("Picker"));
	gtk_container_child_set (GTK_CONTAINER (priv->page_stack), priv->picker, "position", 0, NULL);
	gtk_widget_set_visible (priv->picker, TRUE);

	/* Call the callback to initialise the GtkEntry and to prevent
	 * saving #000000 when saving the white color right away. */
	gcolor3_window_picker_changed (GCOLOR3_COLOR_SELECTION (priv->picker), window);

	g_action_map_add_action_entries (G_ACTION_MAP (window),
					 window_actions,
					 G_N_ELEMENTS (window_actions),
					 window);

	/* Finally, make the color picker the visible stack page. */
	gtk_stack_set_visible_child_name (GTK_STACK (priv->page_stack), "picker");
}

Gcolor3Window *
gcolor3_window_new (Gcolor3Application *application)
{
	g_return_val_if_fail (GCOLOR3_IS_APPLICATION (application), NULL);

	return g_object_new (GCOLOR3_TYPE_WINDOW,
			     "application", application,
			     NULL);
}

void
gcolor3_window_destroy (Gcolor3Window *window, UNUSED gpointer user_data)
{
	g_return_if_fail (window != NULL);
	gtk_widget_destroy (GTK_WIDGET (window));
}
