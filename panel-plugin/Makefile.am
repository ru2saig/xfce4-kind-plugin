AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-DG_LOG_DOMAIN=\"xfce4-kind-plugin\" \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	$(PLATFORM_CPPFLAGS)

#
# kind plugin
#
plugin_LTLIBRARIES = \
	libkind.la

plugindir = \
	$(libdir)/xfce4/panel/plugins

libkind_built_sources = \
	kind-dialog_ui.h

libkind_la_SOURCES = \
	$(libkind_built_sources) \
	kind.c \
	kind.h \
	kind-dialogs.c \
	kind-dialogs.h \
	kind-config.c \
	kind-config.h

libkind_la_CFLAGS = \
	$(LIBXFCE4UTIL_CFLAGS) \
	$(LIBXFCE4UI_CFLAGS) \
	$(LIBXFCE4PANEL_CFLAGS) \
	$(PLATFORM_CFLAGS)

libkind_la_LDFLAGS = \
       -avoid-version \
       -module \
       -no-undefined \
       -export-symbols-regex '^xfce_panel_module_(preinit|init|construct)' \
       $(PLATFORM_LDFLAGS)

libkind_la_LIBADD = \
	$(LIBXFCE4UTIL_LIBS) \
	$(LIBXFCE4UI_LIBS) \
	$(LIBXFCE4PANEL_LIBS)

#
# Desktop file
#
desktopdir = \
	$(datadir)/xfce4/panel/plugins

desktop_DATA = \
	kind.desktop

@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST = \
	kind.desktop.in \
	kind-dialog.glade

BUILT_SOURCES = \
	$(libkind_built_sources)

DISTCLEANFILES = \
	$(libkind_built_sources)

CLEANFILES = \
	$(desktop_DATA)

kind-dialog_ui.h: kind-dialog.glade
	$(AM_V_GEN) xdt-csource --static --strip-comments --strip-content --name=kind_dialog_ui $< >$@
# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
