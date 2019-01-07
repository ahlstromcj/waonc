#*****************************************************************************
# Makefile.am (libwaonc)
#-----------------------------------------------------------------------------
##
# \file          Makefile.am
# \library       libwaonc
# \author        Chris Ahlstrom
# \date          2013-10-19
# \updates       2019-01-06
# \version       $Revision$
# \license       $WAONC_SUITE_GPL_LICENSE$
#
#  	This file is a makefile for the XPCCUT library project.
#
#     This makefile provides the skeleton needed to build the XPCCUT project
#     directory using GNU autotools.
#
#-----------------------------------------------------------------------------

#*****************************************************************************
# Packing targets.
#-----------------------------------------------------------------------------
#
#		Always use Automake in foreign mode (adding foreign to
#		AUTOMAKE_OPTIONS in Makefile.am). Otherwise, it requires too many
#		boilerplate files from the GNU coding standards that aren't useful to
#		us. 
#
#-----------------------------------------------------------------------------

AUTOMAKE_OPTIONS = foreign dist-zip dist-bzip2
MAINTAINERCLEANFILES = Makefile.in Makefile $(AUX_DIST)

#*****************************************************************************
# EXTRA_DIST
#-----------------------------------------------------------------------------
#
#      Extra files in this directory.
#
#      The reference says to use "DISTFILES = ABOUT-NLS".  But vim doesn't
#      highlight that as a keyword, and we don't see reference to it elsewhere
#      on the web.
#
#-----------------------------------------------------------------------------

EXTRA_DIST = README

#*****************************************************************************
#	Packaging.
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
# Installed directories.
#------------------------------------------------------------------------------

prefix = @prefix@
libdir = @xpclibdir@
datadir = @datadir@
datarootdir = @datarootdir@
xpclibdir = @xpclibdir@
xpcdocdir = @xpcdocdir@
xpcdoxygendir = @xpcdoxygendir@
xpcincludedir = @xpcincludedir@

#******************************************************************************
# localedir
#------------------------------------------------------------------------------
#
#  'localedir' is the normal system directory for installed localization
#  files.
#
#------------------------------------------------------------------------------

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

#******************************************************************************
# Local project directories
#------------------------------------------------------------------------------
#
#		top_srcdir    = ../../.. [xpc_suite directory]
#		builddir      = /home/ahlstrom/ca/mls/git/xpc_suite-1.1/debug
#
#------------------------------------------------------------------------------

top_srcdir = @top_srcdir@
builddir = @abs_top_builddir@

#*****************************************************************************
# libtool
#-----------------------------------------------------------------------------

version = $(WAONC_API_MAJOR):$(WAONC_API_MINOR):$(WAONC_API_PATCH)

#*****************************************************************************
# SUBDIRS
#-----------------------------------------------------------------------------
#
#   	SUBDIRS contains the subdirectories of the current directory that must
#   	be built. It must be defined manually; Automake will never guess a
#   	directory is to be built.  It is possible to define it conditionally
#   	so that some directory will be omitted from the build.
#
#     Canonical order:
#
#        SUBDIRS = doc intl lib src po
#			SUBDIRS = m4 include src tests doc man po pkgconfig w32 intl contrib
#
#-----------------------------------------------------------------------------

SUBDIRS = include src

#*****************************************************************************
# DIST_SUBDIRS
#-----------------------------------------------------------------------------
#
#      DIST_SUBDIRS is used by targets that need to recurse into /all/
#      directories, even those which have been conditionally left out of the
#      build.
#
#      Precisely, DIST_SUBDIRS is used by:
#
#         -   make dist
#         -   make distclean
#         -   make maintainer-clean.
#
#      All other recursive targets use SUBDIRS.
#
#-----------------------------------------------------------------------------

DIST_SUBDIRS = $(SUBDIRS)

#*****************************************************************************
# Tests
#-----------------------------------------------------------------------------
#
# 		No tests yet included.
#
#-----------------------------------------------------------------------------

#*****************************************************************************
# all-local
#-----------------------------------------------------------------------------

all-local:
	@echo "Top source-directory 'top_srcdir' is $(top_srcdir)"
	@echo "* * * * * All waonc build items completed * * * * *"

#******************************************************************************
# Debugging targets
#------------------------------------------------------------------------------

show:
	@echo "Install directories:"
	@echo
	@echo "prefix        = $(prefix)"
	@echo "datadir       = $(datadir)"
	@echo "datarootdir   = $(datarootdir)"
	@echo "libdir        = $(libdir)"
	@echo "xpclibdir     = $(xpclibdir)"
	@echo "xpcdocdir     = $(xpcdocdir)"
	@echo "xpcdoxygendir = $(xpcdoxygendir)"
	@echo "xpcincludedir = $(xpcincludedir)"
	@echo "localedir     = $(localedir)"
	@echo
	@echo "Local directories:"
	@echo
	@echo "top_srcdir    = $(top_srcdir) [XPC Basic root directory]"
	@echo "srcdir    		= $(srcdir)"
	@echo "top_builddir  = $(top_builddir)"
	@echo "builddir      = $(builddir)"

#*****************************************************************************
# Makefile.am (libwaonc top-level)
#-----------------------------------------------------------------------------
# vim: ts=3 sw=3 noet ft=automake
#-----------------------------------------------------------------------------
