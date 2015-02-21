#ifndef WAONC_MACROS_H_
#define WAONC_MACROS_H_

/*
 * MACROS for WaoN
 *
 * Copyright (C) 2013-2013 Chris Ahlstrom <ahlstrom@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/**
 * \file          macros.h
 *
 *    This module provides macros for generating simple messages, MIDI
 *    parameters, and more.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2013-11-17
 * \updates       2013-12-24
 * \version       $Revision$
 * \license       GNU GPL
 *
 *    The macros in this file cover:
 *
 *       -  Default values of waonc parameters.
 *       -  MIDI manifest constants.
 *       -  Language-support macros.
 *       -  Error and information output macros.
 */

/**
 * Default Parameters:
 *
 * The cutoff ratio #define provides the log10 of cutoff ratio for scale
 * velocity, and the relative cutoff ration #define  provides the log10 of
 * cutoff ratio relative to average.  The latter value is ignored when
 * abs_flg is true.
 */

#define DEFAULT_CUTOFF_RATIO             (-5.0)
#define DEFAULT_RELATIVE_CUTOFF_RATIO      1.0
#define DEFAULT_FFT_LENGTH                2048
#define DEFAULT_FFT_WINDOW_TYPE           FILTER_WINDOW_HANNING
#define DEFAULT_USE_PHASE                 wtrue
#define DEFAULT_USE_ABSOLUTE_CUTOFF       wtrue
#define DEFAULT_PEAK_THRESHOLD_DISABLED   128

/**
 *    The default top and bottom notes are defined for a 76-key piano.
 */

#define DEFAULT_NOTE_BOTTOM                28                     /* E2 */
#define DEFAULT_NOTE_TOP                  103                     /* G8 */

/**
 * MIDI Manifest Constants:
 *
 *    The first set of macros provides default values for the
 *    minimum/maximum detection code.  We want to be able to know the
 *    highest and lowest MIDI notes that were generated, as a help to
 *    running the application again.
 */

#define MIDI_NOTE_MIN_INITIAL   999
#define MIDI_NOTE_MIN             0
#define MIDI_NOTE_MAX_INITIAL   (-1)
#define MIDI_NOTE_A4             69
#define MIDI_NOTE_MAX           127

#define MIDI_FREQ_A4            440

/**
 *    Defines the maximum number of unique MIDI notes.
 */

#define MIDI_NOTE_COUNT         (MIDI_NOTE_MAX + 1)

/**
 *    Defines the values of the MIDI events "note off" and "note on".
 */

#define MIDI_EVENT_NOTE_OFF       0
#define MIDI_EVENT_NOTE_ON        1

/**
 *    Defines the number of notes in a MIDI octave.
 */

#define MIDI_NOTE_OCTAVE         12

/**
 *    Converts an integer expression to a MIDI note.
 */

#define MIDI_NOTE(x)            ((char) (x))

/**
 *    Defines some standard velocity values.
 */

#define MIDI_VELOCITY_MIN         0
#define MIDI_VELOCITY_HALF       64
#define MIDI_VELOCITY_MAX       127

/**
 *    Defines a value used to indicate that certain values are still
 *    in their uninitialized state.
 */

#define WAON_UNINITIALIZED     (-1)

/**
 *    Defines a value used to indicate that a note function returned an
 *    illegal note.
 */

#define WAON_NOTE_ILLEGAL      (-1)

/**
 * Language macros:
 *
 *    Provides an alternative to NULL.
 */

#ifndef __cplus_plus
#define nullptr      0
#endif

/**
 *    Test for being a valid pointer.
 */

#define not_nullptr(x)     ((x) != nullptr)

/**
 *    Test for being an invalid pointer.
 */

#define is_nullptr(x)      ((x) == nullptr)

/**
 *    A more obvious boolean type.
 */

typedef int wbool_t;

/**
 *    Provides the "false" value of the wbool_t type definition.
 */

#define wfalse    0

/**
 *    Provides the "true" value of the wbool_t type definition.
 */

#define wtrue     1

/**
 * Error/Informational macros:
 *
 *    Provides an error reporting macro (which happens to match Chris's XPC
 *    error function.
 */

#define errprint(x)           fprintf(stderr, "? %s\n", x)

/**
 *    Provides an error reporting macro that requires a sprintf() format
 *    specifier as well.
 */

#define errprintf(fmt, x)     fprintf(stderr, fmt, x)

/**
 *    Provides an information reporting macro (which happens to match
 *    Chris's XPC information function.
 */

#define infoprint(x)          fprintf(stderr, "* %s\n", x)

/**
 *    Provides an error reporting macro that requires a sprintf() format
 *    specifier as well.
 */

#define infoprintf(fmt, x)    fprintf(stderr, fmt, x)

#endif         /* WAONC_MACROS_H_ */

/*
 * macros.h
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
