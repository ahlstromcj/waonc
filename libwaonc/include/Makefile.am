#******************************************************************************
# Makefile.am (waonc/include)
#------------------------------------------------------------------------------
##
# \file     	Makefile.am
# \library     waonc
# \author      Chris Ahlstrom
# \date        2013-10-19
# \updates     2022-05-31
# \version     $Revision$
# \license     $WAONC_SUITE_GPL_LICENSE$
#
#   	This module provides an Automake makefile for the XPC library's
#     ./include/waonc directory.
#
# \note
#   	The normal and re-named configuration header files created are:
#
#     	- 	config.h
#        -  waonc-config.h
#
#		All project share the same file, which is now locating in
#		$(top_srcdir)/include/waonc.
#
#------------------------------------------------------------------------------

#*****************************************************************************
# Packing/cleaning targets
#-----------------------------------------------------------------------------

AUTOMAKE_OPTIONS = foreign dist-zip dist-bzip2
MAINTAINERCLEANFILES = Makefile.in Makefile $(AUX_DIST)

#******************************************************************************
# EXTRA_DIST
#------------------------------------------------------------------------------
#
# 		Note that we have to add "*.h" to ensure these files get distributed.
#
#------------------------------------------------------------------------------

EXTRA_DIST = *.h

#*****************************************************************************
#	Macros
#-----------------------------------------------------------------------------
#
#	This section is recommended by:
#
#		http://olympus.het.brown.edu/cgi-bin/info2www?(gettext)Makefile
#
#-----------------------------------------------------------------------------

PACKAGE = @PACKAGE@
VERSION = @VERSION@

WAONC_API_MAJOR = @WAONC_API_MAJOR@
WAONC_API_MINOR = @WAONC_API_MINOR@
WAONC_API_PATCH = @WAONC_API_PATCH@
WAONC_API_VERSION = @WAONC_API_VERSION@

WAONC_LT_CURRENT = @WAONC_LT_CURRENT@
WAONC_LT_REVISION = @WAONC_LT_REVISION@
WAONC_LT_AGE = @WAONC_LT_AGE@

#******************************************************************************
# Install directories
#------------------------------------------------------------------------------

prefix = @prefix@
libdir = @waonclibdir@
datadir = @datadir@
datarootdir = @datarootdir@
waonclibdir = @waonclibdir@
waoncdocdir = @waoncdocdir@
waoncdoxygendir = @waoncdoxygendir@
waoncincludedir = @waoncincludedir@

#******************************************************************************
# Locale directory
#------------------------------------------------------------------------------
#
#     Not yet used.
#
#------------------------------------------------------------------------------

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

#******************************************************************************
# Local project directories
#------------------------------------------------------------------------------

top_srcdir = @top_srcdir@
builddir = @abs_top_builddir@

#******************************************************************************
# Include directory
#------------------------------------------------------------------------------
#
# 		This define is necessary to cause the proper subdirectory to be made
# 		during installation.  'waoncincludedir' is defined in the configure.ac
# 		script.
#
#  	Without this value, the header files are stored in
#  	"$(prefix)/include/waonc-1.0" instead of
#  	"$(prefix)/include/waonc-1.0/waonc".
#
#------------------------------------------------------------------------------

# pkgincludedir=$(waoncincludedir)/waonc

#******************************************************************************
# Header files
#------------------------------------------------------------------------------

pkginclude_HEADERS = \
 VERSION.h \
 analyse.h \
 ao-wrapper.h \
 fft.h \
 hc.h \
 macros.h \
 memory-check.h \
 midi.h \
 notes.h \
 parameters.h \
 processing.h \
 pv-complex-curses.h \
 pv-complex.h \
 pv-conventional.h \
 pv-ellis.h \
 pv-freq.h \
 pv-loose-lock.h \
 pv-nofft.h \
 snd.h

#******************************************************************************
# uninstall-hook
#------------------------------------------------------------------------------
#
#     We'd like to remove /usr/local/include/waonc-1.0/waonc if it is
#     empty.  However, we don't have a good way to do it yet.
#
#------------------------------------------------------------------------------

uninstall-hook:
	@echo "Note:  you may want to remove $(pkgincludedir) manually"

#******************************************************************************
# Makefile.am (waonc/include)
#------------------------------------------------------------------------------
# vim: ts=3 sw=3 noet ft=automake
#------------------------------------------------------------------------------
