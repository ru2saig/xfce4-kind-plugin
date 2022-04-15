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

#include "libxfce4panel/xfce-panel-plugin.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/libxfce4panel.h>

#include "kind.h"
#include "kind-dialogs.h"
#include "kind-config.h"


#define UPDATE_TIMEOUT 50

/* icons for each key */
static const char *caps_icon[] = {
    "caps_off", "caps_on",
};

static const char *num_icon[] = {
    "num_off", "num_on",
};

static const char *scroll_icon[] = {
    "scroll_off", "scroll_on",
};


/* prototypes */
static void
kind_construct (XfcePanelPlugin *plugin);


/* register the plugin */
XFCE_PANEL_PLUGIN_REGISTER (kind_construct);


static gboolean
kind_update(gpointer data)
{
  KindPlugin *kind = data;
  gboolean caps_on = FALSE;
  gboolean num_on = FALSE;
  gboolean scroll_on = FALSE;

  
  if(kind_config_get_enable_caps_icon(kind->config))
    {
      if(gdk_keymap_get_caps_lock_state(kind->kmap))
        caps_on = TRUE;

      if(kind->caps_on != caps_on)
	{
	  gtk_image_set_from_icon_name (GTK_IMAGE (kind->caps_icon), caps_icon[caps_on], GTK_ICON_SIZE_BUTTON);
	  gtk_image_set_pixel_size (GTK_IMAGE (kind->caps_icon), kind->icon_size);

	  kind->caps_on = caps_on;
	}      
    }


  if(kind_config_get_enable_num_icon(kind->config))
    {
      if( gdk_keymap_get_num_lock_state(kind->kmap))
	num_on = TRUE;
        
      if(kind->num_on != num_on)
	{
	  gtk_image_set_from_icon_name (GTK_IMAGE (kind->num_icon), num_icon[num_on], GTK_ICON_SIZE_BUTTON);
	  gtk_image_set_pixel_size (GTK_IMAGE (kind->num_icon), kind->icon_size);

	  kind->num_on = num_on;
	}

    }
  

  if(kind_config_get_enable_scroll_icon(kind->config))
    {
      if(gdk_keymap_get_scroll_lock_state(kind->kmap))
	scroll_on = TRUE;

      if(kind->scroll_on != scroll_on)
	{
	  gtk_image_set_from_icon_name (GTK_IMAGE (kind->scroll_icon), scroll_icon[scroll_on], GTK_ICON_SIZE_BUTTON);
	  gtk_image_set_pixel_size (GTK_IMAGE (kind->scroll_icon), kind->icon_size);

	  kind->scroll_on = scroll_on;
	}

    }  

  return TRUE;
}

void
kind_save (XfcePanelPlugin *plugin,
             KindPlugin    *kind)
{
  XfceRc *rc;
  gchar  *file;

  /* get the config file location */
  file = xfce_panel_plugin_save_location (plugin, TRUE);

  if (G_UNLIKELY (file == NULL))
    {
       DBG ("Failed to open config file");
       return;
    }

  /* open the config file, read/write */
  rc = xfce_rc_simple_open (file, FALSE);
  g_free (file);

  if (G_LIKELY (rc != NULL))
    {
      /* save the settings */
      DBG(".");
      xfce_rc_write_bool_entry (rc, "enable_caps_icon", kind_config_get_enable_caps_icon(kind->config));
      xfce_rc_write_bool_entry (rc, "enable_num_icon", kind_config_get_enable_num_icon(kind->config));
      xfce_rc_write_bool_entry (rc, "enable_scroll_icon", kind_config_get_enable_scroll_icon(kind->config));


      /* close the rc file */
      xfce_rc_close (rc);
    }
}



static void
kind_read (KindPlugin *kind)
{
  XfceRc      *rc;
  gchar       *file;

  /* get the plugin config file location */
  file = xfce_panel_plugin_save_location (kind->plugin, TRUE);

  if (G_LIKELY (file != NULL))
    {
      /* open the config file, readonly */
      rc = xfce_rc_simple_open (file, TRUE);

      /* cleanup */
      g_free (file);

      if (G_LIKELY (rc != NULL))
        {
          /* read the settings */
          kind_config_set_enable_caps_icon(kind->config, xfce_rc_read_bool_entry (rc, "show_caps_icon", TRUE));
	  kind_config_set_enable_num_icon(kind->config, xfce_rc_read_bool_entry (rc, "show_num_icon", TRUE));
	  kind_config_set_enable_scroll_icon(kind->config, xfce_rc_read_bool_entry (rc, "show_scroll_icon", TRUE));
	  
          /* cleanup */
          xfce_rc_close (rc);

          /* leave the function, everything went well */
          return;
        }
    }

  /* something went wrong, apply default values */
  DBG ("Applying default settings");

  kind_config_set_enable_caps_icon(kind->config, TRUE);
  kind_config_set_enable_num_icon(kind->config, TRUE);
  kind_config_set_enable_scroll_icon(kind->config, TRUE);
}



static void
kind_free (XfcePanelPlugin *plugin,
             KindPlugin    *kind)
{
  GtkWidget *dialog;

  /* check if the dialog is still open. if so, destroy it */
  dialog = g_object_get_data (G_OBJECT (plugin), "dialog");
  if (G_UNLIKELY (dialog != NULL))
    gtk_widget_destroy (dialog);

  if (kind->timeout_id != 0)
    g_source_remove (kind->timeout_id);

  /* destroy the panel widgets */
  if(kind->caps_icon != NULL)
    gtk_widget_destroy(kind->caps_icon);
  if(kind->num_icon != NULL)
    gtk_widget_destroy(kind->num_icon);
  if(kind->scroll_icon != NULL)
    gtk_widget_destroy(kind->scroll_icon);
  gtk_widget_destroy (kind->hvbox);

  /* free the plugin structure */
  g_slice_free (KindPlugin, kind);
}



static void
kind_orientation_changed (XfcePanelPlugin *plugin,
                            GtkOrientation   orientation,
                            KindPlugin    *kind)
{
  /* change the orientation of the box */
  gtk_orientable_set_orientation(GTK_ORIENTABLE(kind->hvbox), orientation);
}



static gboolean
kind_size_changed (XfcePanelPlugin *plugin,
                     gint             size,
                     KindPlugin    *kind)
{
  GtkOrientation orientation;

  /* get the orientation of the plugin */
  orientation = xfce_panel_plugin_get_orientation (plugin);

  /* set the widget size */
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    gtk_widget_set_size_request (GTK_WIDGET (plugin), -1, size);
  else
    gtk_widget_set_size_request (GTK_WIDGET (plugin), size, -1);

  kind->icon_size = size - 4;
  gtk_image_set_pixel_size (GTK_IMAGE (kind->caps_icon), kind->icon_size);
  gtk_image_set_pixel_size (GTK_IMAGE (kind->num_icon), kind->icon_size);
  gtk_image_set_pixel_size (GTK_IMAGE (kind->scroll_icon), kind->icon_size);

  /* we handled the orientation */
  return TRUE;
}



static void
kind_construct (XfcePanelPlugin *plugin)
{
  KindPlugin *kind;
  GtkOrientation  orientation;

  
  /* setup transation domain */
  xfce_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");

  
  /* allocate memory for the plugin structure */
  kind = g_slice_new0 (KindPlugin);

  /* pointer to plugin */
  kind->plugin = plugin;

  // initalize configuration
  kind->config = kind_config_new(xfce_panel_plugin_get_property_base(plugin));

  /* read the user settings */
  kind_read (kind);
  
  /* get the current orientation */
  orientation = xfce_panel_plugin_get_orientation (plugin);


  /* get the kmap */
  kind->kmap = gdk_keymap_get_for_display(gdk_display_get_default());

  // add an event box
  kind->ebox = gtk_event_box_new ();
  gtk_widget_show (kind->ebox);

  kind->hvbox = gtk_box_new(orientation, 2);
  gtk_widget_show(kind->hvbox);
  gtk_container_add(GTK_CONTAINER(kind->ebox), kind->hvbox);
  
  /* set up the indicator icons */
  kind->icon_size = 1;
  kind->caps_on = -1;
  kind->num_on = -1;
  kind->scroll_on = -1;
  
  kind->caps_icon = gtk_image_new();
  kind->num_icon = gtk_image_new();
  kind->scroll_icon = gtk_image_new();

  gtk_widget_show(kind->caps_icon);
  gtk_box_pack_start(GTK_BOX(kind->hvbox), kind->caps_icon, FALSE, FALSE, 0);
  gtk_widget_show(kind->num_icon);
  gtk_box_pack_start(GTK_BOX(kind->hvbox), kind->num_icon, FALSE, FALSE, 0);
  gtk_widget_show(kind->scroll_icon);
  gtk_box_pack_start(GTK_BOX(kind->hvbox), kind->scroll_icon, FALSE, FALSE, 0);

  
  if(kind->timeout_id == 0)
    kind->timeout_id = g_timeout_add(UPDATE_TIMEOUT, kind_update, kind);

  kind_update(kind);
  gtk_container_add (GTK_CONTAINER (plugin), kind->ebox);

  

  /* show the panel's right-click menu on the ebox*/
  xfce_panel_plugin_add_action_widget (plugin, kind->ebox);
  
  
  /* connect plugin signals */
  g_signal_connect (G_OBJECT (plugin), "free-data",
                    G_CALLBACK (kind_free), kind);

  g_signal_connect (G_OBJECT (plugin), "save",
                    G_CALLBACK (kind_save), kind);

  g_signal_connect (G_OBJECT (plugin), "size-changed",
                    G_CALLBACK (kind_size_changed), kind);

  g_signal_connect (G_OBJECT (plugin), "orientation-changed",
                    G_CALLBACK (kind_orientation_changed), kind);

  /* show the configure menu item and connect signal */
  xfce_panel_plugin_menu_show_configure (plugin);
  g_signal_connect (G_OBJECT (plugin), "configure-plugin",
                    G_CALLBACK (kind_configure), kind);

  /* show the about menu item and connect signal */
  xfce_panel_plugin_menu_show_about (plugin);
  g_signal_connect (G_OBJECT (plugin), "about",
                    G_CALLBACK (kind_about), NULL);
}
