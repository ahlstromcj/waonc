#*******************************************************************************
# Makefile.am (m4)
#-------------------------------------------------------------------------------
##
# \file       	Makefile.am
# \library    	waonc
# \author     	Chris Ahlstrom
# \date       	2013-10-19
# \update       2022-05-31
# \version    	$Revision$
# \license    	$XPC_SUITE_GPL_LICENSE$
#
#     This module provides an Automake makefile for the waonc library's
#     main 'm4' directory.
#
#     This makefile provides mostly just a way to make sure the m4 files are
#     included in the 'dist' target.
#
#-------------------------------------------------------------------------------

AUTOMAKE_OPTIONS = foreign dist-zip dist-bzip2
MAINTAINERCLEANFILES = Makefile.in Makefile

PACKAGE = @PACKAGE@
VERSION = @VERSION@
API_VERSION = @API_VERSION@
datarootdir = @datarootdir@
xpcincludedir=@xpcincludedir@

#*******************************************************************************
# EXTRA_DIST
#-------------------------------------------------------------------------------

EXTRA_DIST = ax_prefix_config_h.m4 ax_require_defined.m4 ax_with_curses.m4 \
fftw.m4 gcc-version.m4 gtk-2.0.m4 inttypes.m4 isc-posix.m4 libtool.m4 \
lt~obsolete.m4 ltoptions.m4 ltsugar.m4 ltversion.m4 pkg.m4 samplerate.m4 \
sndfile.m4 threadlib.m4 win32msc.m4 xpc_debug.m4 \
xpc_doxygen.m4 xpc_errorlog.m4 xpc_mingw32.m4 xpc_nullptr.m4 xpc_thisptr.m4

#******************************************************************************
# Makefile.am (m4)
#------------------------------------------------------------------------------
# 	vim: ts=3 sw=3 ft=automake
#------------------------------------------------------------------------------
