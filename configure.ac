dnl ***************************************************************************
dnl configure.ac (waonc)
dnl ---------------------------------------------------------------------------
dnl
dnl \file          configure.ac
dnl \library       waonc
dnl \author        Chris Ahlstrom
dnl \date          2013-10-19
dnl \update        2022-05-31
dnl \version       $Revision$
dnl \license       $WAONC_SUITE_GPL_LICENSE$
dnl
dnl   This script is for the waonc project, a repackaging and fixing of waon.
dnl
dnl ---------------------------------------------------------------------------

AC_PREREQ(2.60)
AC_REVISION($Revision: 0.11.0$)dnl
AC_INIT(waonc, [0.12], [ahlstromcj@users.sourceforge.net])
AC_CONFIG_AUX_DIR([aux-files])
AC_CONFIG_SRCDIR([libwaonc/src/midi.c])

dnl Do we need the following?  No, but it cleans up a lot of warning messages.

AC_CANONICAL_SYSTEM

dnl See http://www.delorie.com/gnu/docs/automake/automake_24.html

AM_INIT_AUTOMAKE
AC_CONFIG_HEADER([include/config.h])

dnl Prepends "WAONC_" to a lot of the macro definitions, for a better
dnl guarantee of avoiding naming conflicts.

AX_PREFIX_CONFIG_H([include/waonc-config.h], [waonc])

dnl New for autoconf 2.60, prevents spurious datarootdir warnings.

AC_DEFUN([AC_DATAROOTDIR_CHECKED])

dnl 1. Package version information.  The first item is the name that covers
dnl    a number of libraries, all of whose names start with "waonc".
dnl    We have to set up a "suite" name; since PACKAGE is defined to be the
dnl    name of the current project, we can't use that value in the package
dnl    configuration files.  It is replaced by WAONC_SUITE_NAME, which covers
dnl    all of the library projects.

PACKAGE=waonc
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE")
AC_SUBST(PACKAGE)

dnl VERSION="0.12.0"
dnl AC_DEFINE_UNQUOTED(VERSION, "$VERSION")
dnl AC_SUBST(VERSION)

WAONC_SUITE_NAME="WAONC"
AC_SUBST([WAONC_SUITE_NAME])

dnl 1.a. Set up version information for this project.
dnl
dnl      This versioning is the libtool version.
dnl      Warning: These values are inserted into libtool's "version-info"
dnl      value, and have to follow libtool rules:
dnl
dnl      Format is 'current:revision:age', where
dnl
dnl      current = Number of the current interface.  If the interface has
dnl      changed, increment this number and set 'revision' to 0.  If the new
dnl      interface has removed elements of the preceding interface, backward
dnl      compatibility has been broken, and you must set 'age' to 0.
dnl
dnl      revision = Implementation number of the most recent interface,
dnl      where 0 is the first.  The runtime loader always uses the library
dnl      with the highest revision.  If you change any source modules,
dnl      increment this number.
dnl
dnl      age = The number of additional interface supported by this library.
dnl      The age must the less than or equal to 'current'.  If the new
dnl      interface is a superset of the preceding interface, 'age' must be
dnl      incremented.

WAONC_API_MAJOR="1"
AC_SUBST([WAONC_API_MAJOR])

WAONC_API_MINOR="1"
AC_SUBST([WAONC_API_MINOR])

WAONC_API_PATCH="0"
AC_SUBST([WAONC_API_PATCH])

WAONC_OFFICIAL_VERSION="$WAONC_API_MAJOR.$WAONC_API_MINOR.$WAONC_API_PATCH"

dnl For version 1.1.0, we reset the libtool version information.

WAONC_LT_CURRENT="0"
AC_SUBST([WAONC_LT_CURRENT])

WAONC_LT_REVISION="0"
AC_SUBST([WAONC_LT_REVISION])

WAONC_LT_AGE="0"
AC_SUBST([WAONC_LT_AGE])

WAONC_LIBTOOL_VERSION="$WAONC_LT_CURRENT.$WAONC_LT_REVISION.$WAONC_LT_AGE"

dnl 1.b. Set up the version strings.  The primary version string of interest
dnl      is WAONC_API_VERSION.

WAONC_API_VERSION="$WAONC_API_MAJOR.$WAONC_API_MINOR"
AC_SUBST([WAONC_API_VERSION])

dnl 1.c. Report the version information in the progress output.

AC_MSG_CHECKING(major version)
AC_MSG_RESULT($WAONC_API_MAJOR)
AC_MSG_CHECKING(minor version)
AC_MSG_RESULT($WAONC_API_MINOR)
AC_MSG_CHECKING(patchlevel)
AC_MSG_RESULT($WAONC_API_PATCH)

dnl 1.d. Set up the suite name and current-project name.

WAONC_PROJECT_NAME="WAONC"
AC_SUBST([WAONC_PROJECT_NAME])

dnl 2. Libtool
dnl
dnl Initialize libtool in the default manner, which turns on shared
dnl libraries if available, and enable static libraries if they do not conflict
dnl with shared libraries.  Also sets the shell variable LIBTOOL_DEPS.
dnl
dnl The latest GNU documentation specifies version 2 of libtool, but Debian
dnl testing provides only 1.5 (Debian unstable now provides 2.2).  So we don't
dnl use these new macros, just the older, deprecated macros.
dnl
dnl LT_PREREQ([1.5])
dnl LT_INIT
dnl AC_SUBST([LIBTOOL_DEPS])
dnl
dnl See <http://www.delorie.com/gnu/docs/libtool/libtool_27.html> for the
dnl usage of LIBTOOL_DEPS.
dnl
dnl Uncomment the following if you want more straightforward debugging.  An
dnl alternative is to specify --disable-shared when running 'configure'.
dnl
dnl AC_DISABLE_SHARED

AC_PROG_LIBTOOL

LT_RELEASE="$WAONC_API_MAJOR.$WAONC_API_MINOR"
AC_SUBST([LT_RELEASE])

LT_CURRENT="$WAONC_API_MAJOR"
AC_SUBST([LT_CURRENT])

LT_REVISION="$WAONC_API_MINOR"
AC_SUBST([LT_REVISION])

LT_AGE="$WAONC_API_PATCH"
AC_SUBST([LT_AGE])

dnl 3. Set up expandable installation directory(ies).
dnl
dnl These directories are for the waonc library.
dnl
dnl We want all of the header files and library files for the whole suite
dnl to be in the same sub-directory, waonc-1.1.  This will avoid clutter and put
dnl all our code in one place.
dnl
dnl      /usr/include/waonc-1.1/waonc
dnl      /usr/lib/waonc-1.1
dnl      /usr/man/man1
dnl      /usr/share/doc/waonc-1.1/doc/dox/html

waoncincludedir="${includedir}/waonc-${WAONC_API_VERSION}"
AC_SUBST(waoncincludedir)

waonclibdir="${libdir}/waonc-${WAONC_API_VERSION}"
AC_SUBST(waonclibdir)

waoncdocdir="${datadir}/doc/waonc-${WAONC_API_VERSION}"
AC_SUBST(waoncdocdir)

waoncdoxygendir="${datadir}/doc/waonc-${WAONC_API_VERSION}"
AC_SUBST(waoncdoxygendir)

dnl 4. Get external flags.

CFLAGS="${CFLAGS}"

dnl 5. Checks for programs.
dnl
dnl 5.a. Plain C support.
dnl
dnl Many AC macros are not needed, since we use AC_PROG_LIBTOOL, which
dnl defines these for us.
dnl
dnl Check for POSIX

AC_ISC_POSIX

dnl Check for the presence of Doxygen.  Not an error to be missing, but
dnl we don't want to try to build documentation if it is not present.
dnl
dnl See http://www.gnu.org/software/autoconf-archive/ax_prog_doxygen.html

AC_PROG_DOXYGEN

dnl 5.b. gcc support.

AC_GCC_VERSION

dnl C++ support.  Run tests in C++.  Note the order; avoids messy warnings.
dnl
dnl AC_LANG_CPLUSPLUS dnl or AC_LANG(C++)
dnl AC_PROG_CXX
dnl AC_PROG_CXXCPP

dnl 5.c. Mingw support.
dnl
dnl Checks for the MingW32 compiler environment. If present, sets shell
dnl variable MINGW32 to 'yes'. If not present, sets MINGW32 to the
dnl empty string.
dnl
dnl Also check to see if the user specified the MingW32 installation
dnl location.  On Debian, the installation is in /usr/i586-mingw32msvc/.
dnl The Mingw versions of the executables start with "i586".  For example,
dnl the compiler is "i586-mingw32msvc-gcc-3.4.5".
dnl
dnl http://www.wxwidgets.org/wiki/index.php/Cross-Compiling_Under_Linux
dnl
dnl is a good place to start learning about building Windows applications.

AC_XPC_MINGW32
AC_ARG_WITH(mingw32,[  --with-mingw32[=DIR]    MingW32 installation location])

dnl Checks for the Cygwin environment. If present, sets shell variable
dnl CYGWIN to 'yes'; if not present, sets CYGWIN to the empty string.

AC_CYGWIN

dnl Clear out the X11 flags for the case where we are cross-compiling
dnl for i586-mingw32 targets, under UNIX.

if test "x$CYGWIN" = "xyes" -o "x$MINGW32" = "xyes" ; then
   X_CFLAGS=""
   X_LIBS=""
fi

dnl 6.0  Top portion of the config.h/waonc-config.h header files.
dnl      The waonc-config.h header file has "WAONC_" prepended to these
dnl      automatically for us.  Also, we can't use
dnl      #define WAONC_API_VERSION "${WAONC_API_VERSION}",
dnl      since the macro call in quotes does /not/ get expanded.
dnl
dnl      Note: The AX_PREFIX_CONFIG_H macro called way above prepends
dnl            "WAONC_" to these values when it generated waonc-config.h.

AH_TOP(

#define VERSION_DATE_SHORT "2022-05-31"
#define VERSION "0.12.0"
#define API_VERSION "0.12"

)

dnl 7. Checks for build configuration.
dnl
dnl 7.a. Compiling with debugging, coverage, or profiling options.
dnl
dnl      --enable-debug
dnl      --enable-coverage
dnl      --enable-profiling

AC_XPC_DEBUGGING

dnl 7.b. Compiling with error-logging suport.

AC_XPC_ERRORLOG

dnl 7.c. Set up for inactive null-pointer checking.  --disable-nullptr
dnl
dnl Use this option only if your code is certain to work, and you need
dnl a little extra speed.

AC_XPC_NULLPTR

dnl libao:  https://svn.xiph.org/branches/release_0_7_1/ao/ao.m4
dnl
dnl Defines AO_CFLAGS and AO_LIBS
dnl
dnl  CFLAGS="$CFLAGS $AO_CFLAGS"
dnl  LIBS="$LIBS $AO_LIBS"
dnl
dnl Yields errors and we're not sure why.

dnl XIPH_PATH_AO(,AC_MSG_ERROR(libao-dev needs to be installed)

AC_CHECK_LIB(ao, ao_initialize, [], [
   echo "Error! You need to install libao-dev."
   exit -1
   ])

dnl This works on Debian, the AC_CHECK_LIB macro does not, probably
dnl since libfttw3 is installed in /usr/lib/x86_64-linux-gnu.
dnl Also, we're trying a more complete m4/fftw.m4 script.

AX_FFTW

dnl AC_CHECK_LIB(fttw3, fttw_execute, [], [
dnl    echo "Error! You need to install libfttw3-dev."
dnl    exit -1
dnl    ])

AX_WITH_CURSES

dnl Requires that libgtk2.0-dev be installed before it will work.
dnl Sigh.  Still doesn't work due to error involving libz.
dnl
dnl AM_PATH_GTK_2_0([2.10.0],,AC_MSG_ERROR([Gtk+ 2.10.0 or higher required.]))

dnl
dnl AC_CHECK_LIB(gtk+-2.0, gtk_status_icon_new, [], [
dnl    echo "Error! You need to install libgtk2.0-dev and its dependencies."
dnl    exit -1
dnl    ])

AC_CHECK_LIB(jack, jack_transport_query, [], [
   echo "Error! You need to install libjack-jack2d-dev or libjack-dev."
   exit -1
   ])

AC_SNDFILE(, [
   echo "Error! You need to install libsndfile1-dev."
   exit -1
   ])

AC_SAMPLERATE(, [
   echo "Error! You need to install libsamplerate0-dev."
   exit -1
   ])

dnl Get access to gtk+ and glib, which aren't always in a standard place.

PKG_CHECK_MODULES([GTK], [gtk+-2.0])
AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])

PKG_CHECK_MODULES([GLIB], [glib-2.0])
AC_SUBST([GLIB_CFLAGS])
AC_SUBST([GLIB_LIBS])

PKG_CHECK_MODULES([GDK_PIXBUF], [gdk-pixbuf-2.0])
AC_SUBST([GDK_PIXBUF_CFLAGS])
AC_SUBST([GDK_PIXBUF_LIBS])

PKG_CHECK_MODULES([JACK], [jack])
AC_SUBST([JACK_CFLAGS])
AC_SUBST([JACK_LIBS])

dnl 8.  Set up other options in the compiler macros.
dnl
dnl Note that the XPC libraries require _REENTRANT to be defined,
dnl as discussed in http://pauillac.inria.fr/~xleroy/linuxthreads/faq.html.
dnl
dnl -W is replaced by -Wextra, which supercedes it.
dnl
dnl Some features of the C code (e.g. NAN support in nan_inf.c) require that
dnl an -std=gnu99 or -std=c99 option be added to the gcc command line.
dnl Apparently this option is (in some manner) in force for C++ code.  For
dnl example, the NAN macro in /usr/include/bits/nan.h was picked up if
dnl /usr/include/math.h was included in a C++ file, but not in a C file,
dnl unless the gnu99 C standard was specified.
dnl
dnl TODO:  work in these flags at some point:
dnl
dnl -march=pentium (?) -O3 -ffast-math \

APIDEF="-DAPI_VERSION=\"$WAONC_API_VERSION\""
WARNFLAGS="-Wall -Wextra -pedantic $WARNINGS"
SPEEDFLAGS="-ffast-math"
COMMONFLAGS="$WARNFLAGS -D_REENTRANT $APIDEF $DBGFLAGS $STACKCHK $NOERRLOG $NOTHISPTR"

dnl In addition, we're going to support C++11 to the extent that the GNU
dnl implementation supports it.  Therefore, "-std=c++11" is specified.
dnl
dnl CXXFLAGS="$CFLAGS $COMMONFLAGS -Wno-variadic-macros -std=c++11"

CFLAGS="$CFLAGS $COMMONFLAGS"

dnl CFLAGS="${CFLAGS} $COMMONFLAGS $XPCCUT_CFLAGS"

dnl Add $WARNINGS_ENABLED to CXXFLAGS if you want to see all warnings, all
dnl the time.

WARNINGS_ENABLED="-Wall"
WARNINGS_DISABLED="-Wno-unused-parameter -Wno-non-virtual-dtor"

dnl 10. Checks for header files.

dnl AC_HEADER_STDBOOL

AC_HEADER_TIME
AC_HEADER_STDC

AC_CHECK_HEADERS([ao/ao.h])
AC_CHECK_HEADERS([stdarg.h stdio.h stddef.h])
AC_CHECK_HEADERS([stdlib.h string.h])
AC_CHECK_HEADERS([limits.h ctype.h sys/time.h time.h errno.h])
AC_CHECK_HEADERS([errno.h sys/sysctl.h])
AC_CHECK_HEADERS([math.h])
AC_CHECK_HEADERS([pthread.h])

AC_CHECK_TYPES([errno_t], [], [], [[#include <errno.h>]])

dnl Checks for typedefs, structures, and compiler characteristics.

AC_C_CONST
AC_TYPE_SIZE_T
AC_STRUCT_TM

dnl Checks for library functions.

AC_FUNC_MALLOC
AC_FUNC_SELECT_ARGTYPES
AC_CHECK_FUNCS([strerror])

dnl 11. Checks for internationalization macros (i18n).
dnl
dnl These checks are derived from the GNU "hello world" project at
dnl
dnl      http://ftp.gnu.org/gnu/hello/

AC_CHECK_HEADERS([libintl.h locale.h])

dnl No internationalization support at present.
dnl
dnl AM_PO_SUBDIRS
dnl AM_GNU_GETTEXT([external])
dnl AM_GNU_GETTEXT_VERSION([0.18.1])

AH_BOTTOM(
   /* Bottom of header config */
)

dnl ./gwaonc

AC_CONFIG_FILES([
 Makefile
 m4/Makefile
 libwaonc/Makefile
 libwaonc/include/Makefile
 libwaonc/src/Makefile
 pvc/Makefile
 waonc/Makefile
 gwaonc/Makefile
 ])

AC_OUTPUT

cat << E_O_F

Then do 'make' to compile the waonc projects.
Do 'make check' or 'make test' to run the tests of the projects.
Do 'make install' to install the libwaonc library.
Do 'make dist', etc. to create gzip, zip, and shar archives.

E_O_F

dnl ***************************************************************************
dnl configure.ac (XPC Basic)
dnl ---------------------------------------------------------------------------
dnl vim: ts=3 sw=3 et ft=config
dnl ---------------------------------------------------------------------------
