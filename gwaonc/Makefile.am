#******************************************************************************
# Makefile.am (waonc)
#------------------------------------------------------------------------------
##
# \file       	Makefile.am
# \library    	libwaonc and gwaonc application
# \author     	Chris Ahlstrom
# \date       	2013-10-27
# \update      2015-02-15
# \version    	$Revision$
# \license    	$XPC_SUITE_GPL_LICENSE$
#
# 		This module provides an Automake makefile for the gwaonc application.
#
#------------------------------------------------------------------------------

#*****************************************************************************
# Packing/cleaning targets
#-----------------------------------------------------------------------------

AUTOMAKE_OPTIONS = foreign dist-zip dist-bzip2
MAINTAINERCLEANFILES = Makefile.in Makefile $(AUX_DIST)

#******************************************************************************
# CLEANFILES
#------------------------------------------------------------------------------

CLEANFILES = *.gc*

#******************************************************************************
#  EXTRA_DIST
#------------------------------------------------------------------------------
#
#  getopt_test.c is not ready and is not included at this time.
#	$(TESTS) is not included because it is derived files.
#
#------------------------------------------------------------------------------

EXTRA_DIST = dl_leaks.supp make-tests README

#******************************************************************************
# Items from configure.ac
#-------------------------------------------------------------------------------
#
#  XPCCUT_LIBS is defined by the PKG_CHECK_MODULES macro.  The xpccut
#  library must be installed officially in order to be used to build the
#  unit-test application of the xpc library.
#
#-------------------------------------------------------------------------------

PACKAGE = @PACKAGE@
VERSION = @VERSION@

#******************************************************************************
# Install directories
#------------------------------------------------------------------------------
#
# 	Not needed, yet, since we won't be installing the app for awhile.
#
#------------------------------------------------------------------------------

prefix = @prefix@
libdir = @xpclibdir@
datadir = @datadir@
datarootdir = @datarootdir@
waoncincludedir = @waoncincludedir@
waonclibdir = @waonclibdir@

#******************************************************************************
# localedir
#------------------------------------------------------------------------------
#
# 	'localedir' is the normal system directory for installed localization
#  files.
#
#------------------------------------------------------------------------------

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

#******************************************************************************
# Local project directories
#------------------------------------------------------------------------------

top_srcdir = @top_srcdir@
builddir = @abs_top_builddir@

libwaoncdir = $(builddir)/libwaonc/src/.libs

#******************************************************************************
# AM_CPPFLAGS [formerly "INCLUDES"]
#------------------------------------------------------------------------------
#
# 	'AM_CPPFLAGS' is the set of directories needed to access all of the
# 	XPC-suite unit-test library header files used in this project.
#
#		Defines where to look for header files.
#
#			-	If we build from the xpc_suite root directory, then $top_srcdir
#           is "../.."
#			-	If we build from the xpc directory, then it is "..".
#			-	If we enter a directory such as "debug", and do "../configure",
#				then it is "../../.."
#
#		"../include" is where the xpc header files are located, and it must
#		be defined even if $top_srcdir is "../..".  So we need two entries,
#		to handle the two possible build situations.
#
#		By convention, we include the "xpc" subdirectory in the C/C++ #include
#		directives, so it is not needed here.
#
#------------------------------------------------------------------------------

# pkg-config --cflags  glib-2.0
#
# -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include 
#
# AM_CPPFLAGS = -I$(top_srcdir)/libwaonc/include -I/usr/include/gtk-2.0 -I/usr/include/glib-2.0

AM_CPPFLAGS = -I$(top_srcdir)/libwaonc/include $(GTK_CFLAGS) $(GLIB_CFLAGS)

#******************************************************************************
# libmath
#------------------------------------------------------------------------------
#
# 		One day, we got errors about sqrt() undefined, which we fixed by
# 		adding -lm.  Then one day we got errors about various items in
# 		sys/stat.h being multiply-defined, and it turned out to be the -lm.
#
# 		We make it (an empty) define for how to handle it more easily.
#
#------------------------------------------------------------------------------

libmath = -lm

#****************************************************************************
# Project-specific library files
#----------------------------------------------------------------------------
#
#	These files are the ones built in the source tree, not the installed
#	ones.
#
#  Sometimes one has to change the order of the libraries in this list.
#
#----------------------------------------------------------------------------

libraries = $(libmath) -L$(libwaoncdir) -lwaonc $(GTK_LIBS) $(GLIB_LIBS)

#****************************************************************************
# Project-specific dependency files
#----------------------------------------------------------------------------
#
#  Provdies the specific list of dependencies, to assure that the make
#  detects all changes, if they are available.
#
#  Since this is our own library and test application, we will assume
#  the xpccut-1.1 project has already been built.
#
#----------------------------------------------------------------------------

# dependencies = $(xpcclibdir)/libxpc.a

#******************************************************************************
# The programs to build
#------------------------------------------------------------------------------

bin_PROGRAMS = gwaonc

#******************************************************************************
# gwaonc
#
#     LIBINTL not used.  If you somehow don't have/want pkg-config, the
#     flags are -lfttw3 and -lsndfile.
#
#------------------------------------------------------------------------------

gwaonc_SOURCES = \
 gtk-color.c \
 gwaon-about.c \
 gwaon.c \
 gwaon-menu.c \
 gwaon-play.c \
 gwaon-wav.c \
 gtk-color.h \
 gwaon-about.h \
 gwaon-menu.h \
 gwaon-play.h \
 gwaon-wav.h

gwaonc_LDFLAGS = \
 `pkg-config --libs fftw3` \
 `pkg-config --libs gtk+-2.0` \
 `pkg-config --libs gdk-pixbuf-2.0` \
 `pkg-config --libs ao` \
 `pkg-config --libs jack` \
 `pkg-config --libs sndfile` \
 `pkg-config --libs samplerate`
gwaonc_LDADD = -lpthread -ldl $(libraries)
gwaonc_DEPENDENCIES = $(dependencies)

#******************************************************************************
# Testing
#------------------------------------------------------------------------------
#
# 	   http://www.gnu.org/software/hello/manual/automake/Simple-Tests.html
#
#------------------------------------------------------------------------------

testsubdir = test-results
TESTS_ENVIRONMENT =
TESTS = test_script

test: check

#******************************************************************************
#  distclean
#------------------------------------------------------------------------------

distclean-local:
	-rm -rf $(testsubdir)

#******************************************************************************
# Makefile.am (gwaonc)
#------------------------------------------------------------------------------
# Local Variables:
# End:
#------------------------------------------------------------------------------
# 	vim: ts=3 sw=3 ft=automake
#------------------------------------------------------------------------------
