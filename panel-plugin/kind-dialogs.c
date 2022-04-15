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


static void
kind_show_help(KindPlugin *kind)
{
  gboolean result = g_spawn_command_line_async ("exo-open --launch WebBrowser " PLUGIN_WEBSITE, NULL); 

  if (G_UNLIKELY (result == FALSE))
    g_warning (_("Unable to open the following url: %s"), PLUGIN_WEBSITE);
}


static void
kind_configuration_changed(KindPlugin* kind)
{
  static const char *caps_icon[] = {
    "caps_off", "caps_on",
  };

  static const char *num_icon[] = {
    "num_off", "num_on",
  };

  static const char *scroll_icon[] = {
    "scroll_off", "scroll_on",
  };

  
  if(kind_config_get_enable_caps_icon(kind->config))
    {
      // add the caps icon in the right place
      if(!kind->caps_icon) 
	{
	  kind->caps_icon = gtk_image_new();	  
	  gtk_box_pack_start(GTK_BOX(kind->hvbox), kind->caps_icon, FALSE, FALSE, 0);
	  // What if multiple of them are removed? What then? What about all of em?
	  gtk_box_reorder_child(GTK_BOX(kind->hvbox), kind->caps_icon, 0);

	  gtk_image_set_from_icon_name (GTK_IMAGE (kind->caps_icon), caps_icon[gdk_keymap_get_caps_lock_state(kind->kmap)], GTK_ICON_SIZE_BUTTON);
	  gtk_image_set_pixel_size (GTK_IMAGE (kind->caps_icon), kind->icon_size);
	  
	  gtk_widget_show(kind->caps_icon);
	}
    }
  else 
    { // remove the icon
      if(kind->caps_icon)
	{
	  gtk_widget_destroy(kind->caps_icon);
	  kind->caps_icon = NULL;
	}
    }


  if(kind_config_get_enable_num_icon(kind->config))
    {
      // add the num icon in the right place
      if(!kind->num_icon) 
	{
	  kind->num_icon = gtk_image_new();	  
	  gtk_box_pack_start(GTK_BOX(kind->hvbox), kind->num_icon, FALSE, FALSE, 0);

	  gtk_box_reorder_child(GTK_BOX(kind->hvbox), kind->num_icon, 1);

	  gtk_image_set_from_icon_name (GTK_IMAGE (kind->num_icon), num_icon[gdk_keymap_get_num_lock_state(kind->kmap)], GTK_ICON_SIZE_BUTTON);
	  gtk_image_set_pixel_size (GTK_IMAGE (kind->num_icon), kind->icon_size);
	  
	  gtk_widget_show(kind->num_icon);
	}
    }
  else 
    { // remove the icon
      if(kind->num_icon)
	{
	  gtk_widget_destroy(kind->num_icon);
	  kind->num_icon = NULL;
	}
    }

    if(kind_config_get_enable_scroll_icon(kind->config))
      {
	// add the scroll icon in the right place
	if(!kind->scroll_icon) 
	  {
	    kind->scroll_icon = gtk_image_new();	  
	    gtk_box_pack_start(GTK_BOX(kind->hvbox), kind->scroll_icon, FALSE, FALSE, 0);

	    gtk_box_reorder_child(GTK_BOX(kind->hvbox), kind->scroll_icon, 2);

	    gtk_image_set_from_icon_name (GTK_IMAGE (kind->scroll_icon), scroll_icon[gdk_keymap_get_scroll_lock_state(kind->kmap)], GTK_ICON_SIZE_BUTTON);
	    gtk_image_set_pixel_size (GTK_IMAGE (kind->scroll_icon), kind->icon_size);
	  
	    gtk_widget_show(kind->scroll_icon);
	  }
      }
    else 
      { // remove the icon
	if(kind->scroll_icon)
	  {
	    gtk_widget_destroy(kind->scroll_icon);
	    kind->scroll_icon = NULL;
	  }
      }
}

static void
kind_response(GtkWidget *dialog,
	      gint response,
	      KindPlugin *kind)
{
  xfce_panel_plugin_unblock_menu(kind->plugin);

  kind_save(kind->plugin, kind);
}

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
  xfce_panel_plugin_block_menu (plugin);
  
  if (gtk_builder_add_from_string (builder, kind_dialog_ui, kind_dialog_ui_length, &error))
    {
      dialog = gtk_builder_get_object (builder, "dialog");
      
      g_return_if_fail(XFCE_IS_TITLED_DIALOG(dialog));


      g_signal_connect (G_OBJECT (dialog), "response",
			G_CALLBACK(kind_response), kind);
      
      button = gtk_builder_get_object(builder, "close-button");
      g_return_if_fail(GTK_IS_BUTTON(button));
      g_signal_connect_swapped (G_OBJECT (button), "clicked",
				G_CALLBACK (gtk_widget_destroy), dialog);

      
      button = gtk_builder_get_object(builder, "help-button");
      g_return_if_fail(GTK_IS_BUTTON(button));
      g_signal_connect(G_OBJECT(button), "clicked",
		       G_CALLBACK(kind_show_help), kind);
      

      g_signal_connect_swapped(G_OBJECT(kind->config), "configuration-changed",
		       G_CALLBACK(kind_configuration_changed), kind);
      

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
