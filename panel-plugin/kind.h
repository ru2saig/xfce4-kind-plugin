/*  $Id$
 *
 *  Copyright (C) 2012 John Doo <john@foo.org>
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

#ifndef __SAMPLE_H__
#define __SAMPLE_H__

G_BEGIN_DECLS

/* plugin structure */
typedef struct
{
  XfcePanelPlugin *plugin;

  /* Plugin */
  GtkWidget       *ebox;
  GtkWidget       *hvbox;
  guint           timeout_id;
  GdkKeymap*      kmap;

  /* Key Indicators */ // TODO: make this with Cario, and recolour instead of switching images?
  GtkWidget       *caps_icon;
  GtkWidget       *num_icon;
  GtkWidget       *scroll_icon;

  gboolean        caps_on;
  gboolean        num_on;
  gboolean        scroll_on;

  gint            icon_size;
  
  /* Settings  */
  gboolean        show_caps_icon;
  gboolean        show_num_icon;
  gboolean        show_scroll_icon;

}
  KindPlugin;



void
kind_save (XfcePanelPlugin *plugin,
	   KindPlugin    *kind);

G_END_DECLS

#endif /* !__SAMPLE_H__ */
