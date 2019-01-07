#******************************************************************************
# Makefile.am (pvc)
#------------------------------------------------------------------------------
##
# \file       	Makefile.am
# \library    	libwaonc and pvc application
# \author     	Chris Ahlstrom
# \date       	2013-10-27
# \update      2019-01-06
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
# Flags and settings
#------------------------------------------------------------------------------
#
#  These are tricky, because some packages use _LIB, versus _LIBS.
#
#------------------------------------------------------------------------------

curses_cflags = @CURSES_CFLAGS@
curses_libs = @CURSES_LIBS@
# fftw3_cflags = @FFTW3_CFLAGS@
fftw3_lib = @FFTW3_LIB@
samplerate_cflags = @SAMPLERATE_CFLAGS@
samplerate_libs = @SAMPLERATE_LIBS@
sndfile_cflags = @SNDFILE_CFLAGS@
sndfile_libs = @SNDFILE_LIBS@

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
# AM_CFLAGS [formerly "INCLUDES"]
#------------------------------------------------------------------------------
#
# 	'AM_CFLAGS' is the set of directories needed to access all of the
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

AM_CFLAGS = -I$(top_srcdir)/libwaonc/include \
 $(curses_cflags) \
 $(samplerate_cflags) \
 $(sndfile_cflags)

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

libraries = $(libmath) -L$(libwaoncdir) -lwaonc

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

bin_PROGRAMS = pvc

#******************************************************************************
# pvc
#------------------------------------------------------------------------------
#
#     LIBINTL not used.  If you somehow don't have/want pkg-config, the
#     flags are -lfttw3 and -lsndfile.
#
#------------------------------------------------------------------------------

pvc_SOURCES = pv.c jack-pv.c jack-pv.h

pvc_LDFLAGS = $(libraries) \
 $(curses_libs) \
 $(fftw3_lib) \
 $(samplerate_libs) \
 $(sndfile_libs)

pvc_LDADD = -lpthread -ldl
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
