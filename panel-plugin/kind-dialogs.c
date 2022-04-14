/*  $Id$
 *
 *  Copyright (C) 2019 John Doo <john@foo.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>

#include <libxfce4ui/libxfce4ui.h>
#include <libxfce4panel/libxfce4panel.h>

#include "kind.h"
#include "kind-dialogs.h"
#include "kind_dialog_ui.h"
#include "kind-config.h"

/* the website url */
#define PLUGIN_WEBSITE "https://github.com/ru2saig/xfce4-kind-plugin/" // TODO: add the link to the README here


// is a new instance of the plgin sent? that is weird?
// TODO: investigate what's going on, and why kind->caps_on ain't showing info
static void
kind_show_help(KindPlugin *kind,
	       GtkWidget* sw)
{
  gboolean result = g_spawn_command_line_async ("exo-open --launch WebBrowser " PLUGIN_WEBSITE, NULL); 

  if (G_UNLIKELY (result == FALSE))
    g_warning (_("Unable to open the following url: %s"), PLUGIN_WEBSITE);
}


static void
kind_configuration_changed(KindConfig* config)
{
  DBG("Configuaration Changed");
}


/* TODO: Why is this happening? How to stop it from happening?
   In file included from /usr/include/glib-2.0/gobject/gobject.h:24,
                 from /usr/include/glib-2.0/gobject/gbinding.h:29,
                 from /usr/include/glib-2.0/glib-object.h:22,
                 from /usr/include/glib-2.0/gio/gioenums.h:28,
                 from /usr/include/glib-2.0/gio/giotypes.h:
		 from /usr/include/glib-2.0/gio/gio.h:26,
                 from /usr/include/gtk-3.0/gdk/gdkapplaunchcontext.h:28,
                 from /usr/include/gtk-3.0/gdk/gdk.h:32,
                 from /usr/include/gtk-3.0/gtk/gtk.h:30,
                 from kind-dialogs.c:25:
kind-dialogs.c: In function 'kind_configure':
/usr/include/glib-2.0/gobject/gtype.h:2458:21: warning: 'dialog' may be used uninitialized in this function [-Wmaybe-uninitialized]
 2458 |     ((ct*) (void *) g_type_check_instance_cast ((GTypeInstance*) ip, gt))
      |                     ^~~~~~~~~~~~~~~~~~~~~~~~~~
kind-dialogs.c:72:12: note: 'dialog' was declared here
   72 |   GObject *dialog, *button, *gtk_switch;
 */

void
kind_configure (XfcePanelPlugin *plugin,
                  KindPlugin    *kind)
{
  GtkBuilder *builder;
  GObject *dialog, *button, *gtk_switch;
  GError *error = NULL;

  /* setup the dialog */
  if (xfce_titled_dialog_get_type () == 0)
    return;

  builder = gtk_builder_new ();
  if (gtk_builder_add_from_string (builder, kind_dialog_ui, kind_dialog_ui_length, &error))
    {
      dialog = gtk_builder_get_object (builder, "dialog");

      g_return_if_fail(XFCE_IS_TITLED_DIALOG(dialog));

      // TODO: call kind-save somewhere. Perhaps here
      button = gtk_builder_get_object(builder, "close-button");
      g_return_if_fail(GTK_IS_BUTTON(button));
      g_signal_connect_swapped (G_OBJECT (button), "clicked",
				G_CALLBACK (gtk_widget_destroy), dialog);

      button = gtk_builder_get_object(builder, "help-button");
      g_return_if_fail(GTK_IS_BUTTON(button));
      g_signal_connect(G_OBJECT(button), "clicked",
		       G_CALLBACK(kind_show_help), kind);


      g_signal_connect(G_OBJECT(kind->config), "configuration-changed",
		       G_CALLBACK(kind_configuration_changed), kind->config);
      
      // TODO: implement a callback that checks for this property, and then loads/unloads the icon accordingly. Check NOTES for more info
      gtk_switch = gtk_builder_get_object(builder, "caps_lock_show");
      g_return_if_fail(GTK_IS_SWITCH(gtk_switch));
      g_object_bind_property(G_OBJECT(kind->config), "enable_caps_icon",
			     G_OBJECT(gtk_switch), "state",
			     G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);

      gtk_switch = gtk_builder_get_object(builder, "num_lock_show");
      g_return_if_fail(GTK_IS_SWITCH(gtk_switch));
      g_object_bind_property(G_OBJECT(kind->config), "enable_num_icon",
			     G_OBJECT(gtk_switch), "state",
			     G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);

      gtk_switch = gtk_builder_get_object(builder, "scroll_lock_show");
      g_return_if_fail(GTK_IS_SWITCH(gtk_switch));
      g_object_bind_property(G_OBJECT(kind->config), "enable_scroll_icon",
			     G_OBJECT(gtk_switch), "state",
			     G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);
      
    }

  
  if(G_UNLIKELY(builder == NULL))
    return;
  
  gtk_widget_show(GTK_WIDGET(dialog));

}



void
kind_about (XfcePanelPlugin *plugin)
{
  /* about dialog code. you can use the GtkAboutDialog
   * or the XfceAboutInfo widget */
  GdkPixbuf *icon;

  const gchar *auth[] =
    {
      "Xfce development team <xfce4-dev@xfce.org>", // TODO: change this to disroot email
      NULL
    };

  icon = xfce_panel_pixbuf_from_source ("xfce4-kind-plugin", NULL, 32);
  gtk_show_about_dialog (NULL,
                         "logo",         icon,
                         "license",      xfce_get_license_text (XFCE_LICENSE_TEXT_GPL),
                         "version",      PACKAGE_VERSION,
                         "program-name", PACKAGE_NAME,
                         "comments",     _("Keyboard indicators for xfce4-panel"),
                         "website",      PLUGIN_WEBSITE,
                         "copyright",    _("Copyright \xc2\xa9 2022 Gurusai Nidhish [ru2saig]\n"),
                         "authors",      auth,
                         NULL);

  if (icon)
    g_object_unref (G_OBJECT (icon));
}
