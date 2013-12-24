lib_LTLIBRARIES = connectionsPerIP.la

connectionsPerIP_la_SOURCES = connectionsPerIP.cpp
connectionsPerIP_la_CXXFLAGS= -I$(top_srcdir)/include -I$(top_srcdir)/plugins/plugin_utils
connectionsPerIP_la_LDFLAGS = -module -avoid-version -shared
connectionsPerIP_la_LIBADD = $(top_builddir)/plugins/plugin_utils/libplugin_utils.la

AM_CPPFLAGS = $(CONF_CPPFLAGS)
AM_CFLAGS = $(CONF_CFLAGS)
AM_CXXFLAGS = $(CONF_CXXFLAGS)

EXTRA_DIST = \
	README.txt \
	connectionsPerIP.def \
	connectionsPerIP.sln \
	connectionsPerIP.vcxproj

MAINTAINERCLEANFILES =	\
	Makefile.in
