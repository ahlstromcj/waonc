#******************************************************************************
# Makefile.am (waonc)
#------------------------------------------------------------------------------
##
# \file       	Makefile.am
# \library    	libwaonc and waonc application
# \author     	Chris Ahlstrom
# \date       	2013-10-26
# \update      2022-05-31
# \version    	$Revision$
# \license    	$XPC_SUITE_GPL_LICENSE$
#
# 		This module provides an Automake makefile for the waonc application.
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
#------------------------------------------------------------------------------

AM_CPPFLAGS = -I$(top_srcdir)/libwaonc/include

#******************************************************************************
# libmath
#------------------------------------------------------------------------------
#
# 		-lm
#
#------------------------------------------------------------------------------

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

libraries = -lpthread -ldl -Wl,--start-group -lm -L$(libwaoncdir) -lwaonc -lncursesw -ltinfo $(FFTW_LIBS) $(SNDFILE_LIBS) $(SAMPLERATE_LIBS) -Wl,--end-group

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

dependencies = $(libwaoncdir)/libwaonc.a

#******************************************************************************
# The programs to build
#------------------------------------------------------------------------------

bin_PROGRAMS = waonc

#******************************************************************************
# waonc
#
#     LIBINTL not used.  If you somehow don't have/want pkg-config, the
#     flags are -lfttw3 and -lsndfile.
#
#     -L`ncurses5-config --libdir`
#
#  `ncurses6-config --libs`
#------------------------------------------------------------------------------

waonc_SOURCES = main.c ../include/analyse.h ../include/midi.h \
 ../include/parameters.h ../include/processing.h

waonc_LDFLAGS = -Wl,--copy-dt-needed-entries -Wl,-Bsymbolic-functions $(libraries)
waonc_DEPENDENCIES = $(dependencies)

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
# Makefile.am (waonc)
#------------------------------------------------------------------------------
# 	vim: ts=3 sw=3 ft=automake
#------------------------------------------------------------------------------
