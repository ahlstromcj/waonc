#*****************************************************************************
# Makefile.am (waonc)
#-----------------------------------------------------------------------------
##
# \file        Makefile.am
# \library     waonc
# \author      Chris Ahlstrom
# \date        2013-10-19
# \updates     2013-10-27
# \version     $Revision$
# \license     $WAONC_SUITE_GPL_LICENSE$
#
#     This is the top-level project makefile for the waonc projects.
#
#     This makefile provides the skeleton needed to build the library
#     and application directories using GNU autotools.
#
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Packing targets.
#-----------------------------------------------------------------------------

AUTOMAKE_OPTIONS = foreign dist-zip dist-bzip2
MAINTAINERCLEANFILES = Makefile.in Makefile $(AUX_DIST)

#*****************************************************************************
# Extra files in the top-level directory
#-----------------------------------------------------------------------------

EXTRA_DIST = bootstrap build pack README TODO VERSION

#*****************************************************************************
# Packaging.
#-----------------------------------------------------------------------------
#
#     This section is recommended for the top-level Makefile.am by the
#     reference.
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

#*****************************************************************************
# Installed directories.
#-----------------------------------------------------------------------------
#
# 		The 'libdir' define is necessary to cause the proper subdirectory to
# 		be made during installation.  'waonclibdir' is defined in the
# 		configure.ac script.  The 'libdir' define is needed for work with
# 		libtool.  Not sure about 'pkglibdr'.
#
# 				pkglibdir=$(waonclibdir)
#
#	Directories and macros:
#
#		prefix        = /usr/local
#		libdir        = /usr/local/lib/waonc
#		datadir       = /usr/local/share
#		datarootdir   = /usr/local/share
#		waonclibdir     = /usr/local/lib/waonc
#		waoncdocdir     = /usr/local/share/doc/waonc-1.1
#		waoncdoxygendir = doc
#		waoncincludedir = /usr/local/include/waonc-1.1
#		localedir     = /usr/local/share/locale
#
#  'localedir' is the normal system directory for installed localization
#  files.
#
#-----------------------------------------------------------------------------

prefix = @prefix@
libdir = @waonclibdir@
datadir = @datadir@
datarootdir = @datarootdir@
waoncdocdir = @waoncdocdir@
waoncdoxygendir = @waoncdoxygendir@
waoncincludedir = @waoncincludedir@
waonclibdir = @waonclibdir@
localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

#******************************************************************************
# Local project directories
#------------------------------------------------------------------------------
#
#		top_srcdir    = ../../.. [XPC Basic directory]
#		builddir      = /home/ahlstrom/ca/mls/git/XPC Basic-1.1/debug
#
#------------------------------------------------------------------------------

top_srcdir = @top_srcdir@
builddir = @abs_top_builddir@

#******************************************************************************
# aclocal support
#------------------------------------------------------------------------------

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

#*****************************************************************************
# libtool
#-----------------------------------------------------------------------------
#	${SHELL} $(top_srcdir)/config.status --recheck
#-----------------------------------------------------------------------------

LIBTOOL_DEPS = @LIBTOOL_DEPS@

libtool: $(LIBTOOL_DEPS)
	$(top_srcdir)/config.status --recheck

#*****************************************************************************
# SUBDIRS
#-----------------------------------------------------------------------------
#
# SUBDIRS = m4 include waonc man doc pkgconfig po
#
#-----------------------------------------------------------------------------

SUBDIRS = m4 libwaonc pvc waonc gwaonc

#*****************************************************************************
# DIST_SUBDIRS
#-----------------------------------------------------------------------------

# DISTFILES = ABOUT-NLS

DIST_SUBDIRS = $(SUBDIRS)

#*****************************************************************************
# Tests
#-----------------------------------------------------------------------------
#
#      Uncomment these items only if your project does not support "make
#      check" or "make test".
#
# .PHONY: test check
#
# test: check
#
# check:
#   @echo "'make check' is not implemented in the waonc library."
#
#-----------------------------------------------------------------------------

#*****************************************************************************
# all-local
#-----------------------------------------------------------------------------

all-local:
	@echo "Top source-directory 'top_srcdir' is $(top_srcdir)"
	@echo "* * * * * All build items completed * * * * *"

#******************************************************************************
# Debugging targets
#------------------------------------------------------------------------------

show:
	@echo "Install directories:"
	@echo
	@echo "prefix           = $(prefix)"
	@echo "datadir          = $(datadir)"
	@echo "datarootdir      = $(datarootdir)"
	@echo "libdir           = $(libdir)"
	@echo "waonclibdir      = $(waonclibdir)"
	@echo "waoncdocdir      = $(waoncdocdir)"
	@echo "waoncdoxygendir  = $(waoncdoxygendir)"
	@echo "waoncincludedir  = $(waoncincludedir)"
	@echo "localedir        = $(localedir)"
	@echo
	@echo "Local directories:"
	@echo
	@echo "top_srcdir       = $(top_srcdir) [project root directory]"
	@echo "srcdir           = $(srcdir)"
	@echo "top_builddir     = $(top_builddir)"
	@echo "builddir         = $(builddir)"

showver:
	@echo "PACKAGE            = $(PACKAGE)"
	@echo "VERSION            = $(VERSION)"
	@echo "WAONC_API_MAJOR    = $(WAONC_API_MAJOR)"
	@echo "WAONC_API_MINOR    = $(WAONC_API_MINOR)"
	@echo "WAONC_API_PATCH    = $(WAONC_API_PATCH)"
	@echo "WAONC_API_VERSION  = $(WAONC_API_VERSION)"
	@echo "WAONC_LT_CURRENT   = $(WAONC_LT_CURRENT)"
	@echo "WAONC_LT_REVISION  = $(WAONC_LT_REVISION)"
	@echo "WAONC_LT_AGE       = $(WAONC_LT_AGE)"

#****************************************************************************
# Makefile.am (waonc top-level)
#----------------------------------------------------------------------------
# Local Variables:
# End:
#------------------------------------------------------------------------------
# vim: ts=3 sw=3 ft=automake
#----------------------------------------------------------------------------
