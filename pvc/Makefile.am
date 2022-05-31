#******************************************************************************
# Makefile.am (pvc)
#------------------------------------------------------------------------------
##
# \file       	Makefile.am
# \library    	libwaonc and pvc application
# \author     	Chris Ahlstrom
# \date       	2013-10-27
# \update      2022-05-31
# \version    	$Revision$
# \license    	$XPC_SUITE_GPL_LICENSE$
#
# 		This module provides an Automake makefile for the pvc application.
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
#  CURSES_LIBS
#  CURSES_CFLAGS
#
#----------------------------------------------------------------------------

libraries = -lpthread -ldl -lm -L$(libwaoncdir) -lwaonc -lncursesw -ltinfo $(FFTW_LIBS) $(SNDFILE_LIBS) $(SAMPLERATE_LIBS)

#****************************************************************************
# Project-specific dependency files
#----------------------------------------------------------------------------
#
#  Provides the specific list of dependencies, to assure that the make
#  detects all changes, if they are available.
#
#----------------------------------------------------------------------------

#******************************************************************************
# The programs to build
#------------------------------------------------------------------------------

bin_PROGRAMS = pvc

#******************************************************************************
# pvc
#------------------------------------------------------------------------------
#
#     LIBINTL not used.  If you somehow don't have/want pkg-config, the
#     flags are -lfttw3 and -lsndfile.
#
#     ncurses5-config --libs --> -Wl,-Bsymbolic-functions -lncurses -ltinfo
#
#     pvc_LDFLAGS = -Wl,--copy-dt-needed-entries $(ncurses6-config --libs)
#
#------------------------------------------------------------------------------

pvc_SOURCES = pv.c jack-pv.c jack-pv.h
pvc_LDFLAGS = -Wl,--copy-dt-needed-entries -Wl,-Bsymbolic-functions $(libraries)
pvc_DEPENDENCIES = $(dependencies)

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
# Makefile.am (pvc)
#------------------------------------------------------------------------------
# 	vim: ts=3 sw=3 ft=automake
#------------------------------------------------------------------------------
