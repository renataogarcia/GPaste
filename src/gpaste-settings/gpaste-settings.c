/*
 *      This file is part of GPaste.
 *
 *      Copyright 2013 Marc-Antoine Perennou <Marc-Antoine@Perennou.com>
 *
 *      GPaste is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      GPaste is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with GPaste.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gpaste-settings-ui-notebook.h"

#include <glib/gi18n.h>

static void
init (GApplication *app,
      gpointer      user_data)
{
    gtk_application_add_window (GTK_APPLICATION (app), GTK_WINDOW (user_data));
    gtk_widget_show_all (GTK_WIDGET (user_data));
}

static void
quit (GtkWidget *widget G_GNUC_UNUSED,
      GdkEvent  *event  G_GNUC_UNUSED,
      gpointer   user_data)
{
    g_application_quit (G_APPLICATION (user_data));
}

int
main (int argc, char *argv[])
{
    bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);

    gtk_init (&argc, &argv);
    g_object_set (gtk_settings_get_default (), "gtk-application-prefer-dark-theme", TRUE, NULL);
    
    GtkApplication *app = gtk_application_new ("org.gnome.GPaste.Settings", G_APPLICATION_FLAGS_NONE);
    GtkWidget *window = gtk_widget_new (GTK_TYPE_WINDOW,
                                        "type", GTK_WINDOW_TOPLEVEL,
                                        "title", _("GPaste daemon settings"),
                                        "window-position", GTK_WIN_POS_CENTER,
                                        "resizable", FALSE,
                                        NULL);
    GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    GtkContainer *box = GTK_CONTAINER (vbox);
    GPasteSettingsUiNotebook *notebook = g_paste_settings_ui_notebook_new ();

    g_paste_settings_ui_notebook_fill (notebook);
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET (notebook));

    GtkWidget *close_button = gtk_widget_new (GTK_TYPE_BUTTON,
                                              "label", _("Close"),
                                              "margin", 12,
                                              "margin-top", 0,
                                              NULL);

    gtk_widget_set_halign (close_button, GTK_ALIGN_END);
    gtk_container_add (box, close_button);
    gtk_container_add (GTK_CONTAINER (window), vbox);

    gulong activate_signal = g_signal_connect (app, "activate", G_CALLBACK (init), window);
    gulong quit_signal = g_signal_connect(close_button, "button-press-event", G_CALLBACK (quit), app);

    GApplication *gapp = G_APPLICATION (app);
    GError *error = NULL;

    g_application_register (gapp, NULL, &error);

    if (error)
    {
        fprintf (stderr, "%s\n", _("Failed to register the gtk application."));
        g_error_free (error);
        return 1;
    }

    int ret =  g_application_run (gapp, argc, argv);

    g_signal_handler_disconnect (close_button, quit_signal);
    g_signal_handler_disconnect (app, activate_signal);

    return ret;
}
