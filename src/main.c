/* Gcolor3 main
 *
 * Copyright (C) 2015-2016 Jente Hidskes <hjdskes@gmail.com>
 *
 * Author: Jente Hidskes <hjdskes@gmail.com>
 *
 * Modeled after eog's main.c
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
#include "utils.h"

#include <stdlib.h>
#include <glib/gi18n.h>

#include "gcolor3-application.h"

static gboolean
_print_version_and_exit (UNUSED const gchar *name,
			 UNUSED const gchar *value,
			 UNUSED gpointer     user_data,
			 UNUSED GError     **error)
{
	g_print ("%s %s\n", PACKAGE_NAME, PACKAGE_VERSION);
	exit (EXIT_SUCCESS);
	return TRUE;
}

static const GOptionEntry options[] = {
	{ "color", 'c', 0, G_OPTION_ARG_STRING, &gcolor3_cli_arg_color, N_("The color to default on upon launch"), N_("HEXCOLOR")},
	{ "version", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, _print_version_and_exit, N_("Show the application’s version"), NULL },
	{ NULL }
};

static gboolean
_parse_options (int argc, char **argv)
{
	GOptionContext *context;
	GError *error = NULL;
	gboolean succes;

	context = g_option_context_new (_("— pick a color from the picker or the screen"));
	g_option_context_add_main_entries (context, options, GETTEXT_PACKAGE);
	/* This initialises GTK during parsing. */
	g_option_context_add_group (context, gtk_get_option_group (FALSE));

	succes = g_option_context_parse (context, &argc, &argv, &error);
	g_option_context_free (context);

	if (!succes) {
		gchar *help;

		help = g_strdup_printf (_("Run “%s --help” to see a full "
					"list of available command line "
					"options"), argv[0]);
		g_printerr ("%s\n%s\n", error->message, help);

		g_clear_error (&error);
		g_free (help);

		return FALSE;
	}

  // Check if color argument is correct
  if (g_utf8_strlen (gcolor3_cli_arg_color, -1) > 0) {
    is_gcolor3_cli_arg_color = TRUE;
    GdkRGBA *temp;
    if (!g_str_has_prefix (gcolor3_cli_arg_color, "#")) {
      gcolor3_cli_arg_color = g_strconcat ("#", gcolor3_cli_arg_color, NULL);
    }
    if (!gdk_rgba_parse (temp, gcolor3_cli_arg_color)) {
	  	gchar *help;
	  	help = g_strdup_printf (_("Run “%s --help” to see a full "
	  				"list of available command line "
	  				"options"), argv[0]);
	  	  g_printerr ("Invalid color argument.\n%s\n", help);

	  	g_free (help);
      g_free (temp);
      return FALSE;
    }
    g_free (temp);
  }

	return TRUE;
}

int
main (int argc, char **argv)
{
	Gcolor3Application *application;
	int status;

	bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	if (!_parse_options (argc, argv)) {
		return EXIT_FAILURE;
	}

	application = gcolor3_application_new ();
	status = g_application_run (G_APPLICATION (application), argc, argv);
	g_object_unref (application);

	return status;
}

