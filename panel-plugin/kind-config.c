/*  $Id$
 *
 *  Copyright (C) 2022 Nidhish [ru2saig] <develruusaig@disroot.org>
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
#include "kind-config.h"
#define DEFAULT_ENABLE_ICON TRUE

static void kind_config_get_property   (GObject          *object,
					guint             prop_id,
					GValue           *value,
					GParamSpec       *pspec);
static void kind_config_set_property   (GObject          *object,
					guint             prop_id,
					const GValue     *value,
					GParamSpec       *pspec);

struct _KindConfigClass
{
  GObjectClass __parent__;
};

struct _KindConfig
{
  GObject __parent__;

  /* Settings  */
  gboolean        enable_caps_icon;
  gboolean        enable_num_icon;
  gboolean        enable_scroll_icon;

  // TODO: allow changing the indicator's position
};



enum
  {
    PROP_0,
    PROP_ENABLE_CAPS_ICON,
    PROP_ENABLE_NUM_ICON,
    PROP_ENABLE_SCROLL_ICON,
  };

enum
  {
    CONFIGURATION_CHANGED,
    LAST_SIGNAL,
  };

static guint kind_config_signals [LAST_SIGNAL] = { 0, };

G_DEFINE_TYPE(KindConfig, kind_config, G_TYPE_OBJECT)


KindConfig*
kind_config_new(const gchar *property_base)
{
  KindConfig *config;

  config = g_object_new(TYPE_KIND_CONFIG, NULL);

  return config;
}
  
static void
kind_config_init(KindConfig *config)
{
  config->enable_caps_icon = DEFAULT_ENABLE_ICON;
  config->enable_num_icon = DEFAULT_ENABLE_ICON;
  config->enable_scroll_icon = DEFAULT_ENABLE_ICON;
}

static void
kind_config_class_init(KindConfigClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->get_property = kind_config_get_property;
  gobject_class->set_property = kind_config_set_property;

  g_object_class_install_property(gobject_class,
				  PROP_ENABLE_CAPS_ICON,
				  g_param_spec_boolean("enable-caps-icon", NULL, NULL,
						       DEFAULT_ENABLE_ICON,
						       G_PARAM_READWRITE |
						       G_PARAM_STATIC_STRINGS));

  g_object_class_install_property(gobject_class,
				  PROP_ENABLE_NUM_ICON,
				  g_param_spec_boolean("enable-num-icon", NULL, NULL,
						       DEFAULT_ENABLE_ICON,
						       G_PARAM_READWRITE |
						       G_PARAM_STATIC_STRINGS));

  g_object_class_install_property(gobject_class,
				  PROP_ENABLE_SCROLL_ICON,
				  g_param_spec_boolean("enable-scroll-icon", NULL, NULL,
						       DEFAULT_ENABLE_ICON,
						       G_PARAM_READWRITE |
						       G_PARAM_STATIC_STRINGS));

  kind_config_signals[CONFIGURATION_CHANGED] =
    g_signal_new(g_intern_static_string("configuration-changed"),
		 G_TYPE_FROM_CLASS(gobject_class),
		 G_SIGNAL_RUN_LAST,
		 0, NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
  

}

static void
kind_config_get_property(GObject *object,
			 guint prop_id,
			 GValue *value,
			 GParamSpec *pspec)
{
  KindConfig *config = KIND_CONFIG(object);

  switch(prop_id)
    {
    case PROP_ENABLE_CAPS_ICON:
      g_value_set_boolean(value, config->enable_caps_icon);
      break;

    case PROP_ENABLE_NUM_ICON:
      g_value_set_boolean(value, config->enable_num_icon);
      break;

    case PROP_ENABLE_SCROLL_ICON:
      g_value_set_boolean(value, config->enable_scroll_icon);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;    
    }
  
}

static void
kind_config_set_property(GObject *object,
			 guint prop_id,
			 const GValue *value,
			 GParamSpec *pspec)
{
  KindConfig *config = KIND_CONFIG(object);
  gboolean val_bool;

  switch(prop_id)
    {
    case PROP_ENABLE_CAPS_ICON:
      val_bool = g_value_get_boolean(value);
      if(config->enable_caps_icon != val_bool)
	{
	  config->enable_caps_icon = val_bool;
	  g_object_notify(G_OBJECT(config), "enable-caps-icon");
	  g_signal_emit(G_OBJECT(config), kind_config_signals[CONFIGURATION_CHANGED], 0);
	}
      break;

    case PROP_ENABLE_NUM_ICON:
      val_bool = g_value_get_boolean(value);
      if(config->enable_num_icon != val_bool)
	{
	  config->enable_num_icon = val_bool;
	  g_object_notify(G_OBJECT(config), "enable-num-icon");
	  g_signal_emit(G_OBJECT(config), kind_config_signals[CONFIGURATION_CHANGED], 0);
	}
      break;

    case PROP_ENABLE_SCROLL_ICON:
      val_bool = g_value_get_boolean(value);
      if(config->enable_scroll_icon != val_bool)
	{
	  config->enable_scroll_icon = val_bool;
	  g_object_notify(G_OBJECT(config), "enable-scroll-icon");
	  g_signal_emit(G_OBJECT(config), kind_config_signals[CONFIGURATION_CHANGED], 0);
	}
      break;
    }
  
}

gboolean
kind_config_get_enable_caps_icon(KindConfig* config)
{
  g_return_val_if_fail(IS_KIND_CONFIG(config), DEFAULT_ENABLE_ICON);
  return config->enable_caps_icon;
}

gboolean
kind_config_get_enable_num_icon(KindConfig* config)
{
  g_return_val_if_fail(IS_KIND_CONFIG(config), DEFAULT_ENABLE_ICON);
  return config->enable_num_icon;
}

gboolean
kind_config_get_enable_scroll_icon(KindConfig* config)
{
  g_return_val_if_fail(IS_KIND_CONFIG(config), DEFAULT_ENABLE_ICON);
  return config->enable_scroll_icon;
}

void
kind_config_set_enable_caps_icon(KindConfig* config, gboolean value)
{
  config->enable_caps_icon = value;
}

void
kind_config_set_enable_num_icon(KindConfig* config, gboolean value)
{
  config->enable_num_icon = value;
}

void
kind_config_set_enable_scroll_icon(KindConfig* config, gboolean value)
{
  config->enable_scroll_icon = value;
}
