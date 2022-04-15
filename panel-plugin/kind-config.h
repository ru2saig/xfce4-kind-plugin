/*  $Id$
 *
 *  Copyright (C) 2012 Nidhish [ru2saig] <develruusaig@disroot.org>
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

#ifndef __KIND_CONFIG_H__
#define __KIND_CONFIG_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _KindConfig KindConfig;
typedef struct _KindConfigClass KindConfigClass;


#define TYPE_KIND_CONFIG (kind_config_get_type())
#define KIND_CONFIG(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_KIND_CONFIG, KindConfig))
#define KIND_CONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_KIND_CONFIG, KindConfigClass))
#define IS_KIND_CONFIG(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_KIND_CONFIG))
#define IS_KIND_CONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_KIND_CONFIG))
#define KIND_CONFIG_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_KIND_CONFIG, KindConfigClass))

GType kind_config_get_type(void) G_GNUC_CONST;
KindConfig *kind_config_new(const gchar *property_base);

gboolean kind_config_get_enable_caps_icon(KindConfig *config);
gboolean kind_config_get_enable_num_icon(KindConfig *config);
gboolean kind_config_get_enable_scroll_icon(KindConfig *config);

void kind_config_set_enable_caps_icon(KindConfig *config, gboolean value);
void kind_config_set_enable_num_icon(KindConfig *config, gboolean value);
void kind_config_set_enable_scroll_icon(KindConfig *config, gboolean value);

G_END_DECLS
#endif /** !__KIND_CONFIG_H **/
